// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "mainwindow.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QAction>
#include <QCoreApplication>
#include <QItemSelectionModel>
#include <QList>
#include <QListView>
#include <QPushButton>
#include <QTableView>
#include <QToolBox>
#include <QVariant>

#include "mainwindow/imp.hpp"
#include "otwrap.hpp"
#include "ui_mainwindow.h"
#include "util/focuser.hpp"
#include "widgets/blockchainchooser.hpp"
#include "widgets/licenses.hpp"
#include "widgets/mainwindow/chaintoolboxmanager.hpp"
#include "widgets/showseed.hpp"

namespace metier::widget
{
using OTModel = opentxs::ui::AccountListQt;

MainWindow::MainWindow(QWidget* parent, OTWrap& ot) noexcept
    : QMainWindow(parent)
    , imp_p_(std::make_unique<Imp>(this, ot))
    , imp_(*imp_p_)
{
    setWindowTitle(METIER_APPSTREAM_NAME);
    auto* quit = imp_.ui_->action_file_quit;
    auto* bc = imp_.ui_->action_settings_blockchain;
    auto* words = imp_.ui_->action_settings_recovery_phrase;
    auto* bcdone = imp_.blockchains_->Ok();
    auto* license = imp_.ui_->action_help_opensource;
    auto* toolbox = imp_.ui_->moneyToolbox;
    auto* prog = imp_.ui_->syncProgress;
    connect(&ot, &OTWrap::nymReady, this, &MainWindow::initModels);
    connect(&ot, &OTWrap::readyForMainWindow, this, &MainWindow::updateToolbox);
    connect(&ot, &OTWrap::chainsChanged, this, &MainWindow::updateToolbox);
    connect(quit, &QAction::triggered, []() { QCoreApplication::exit(); });
    connect(bc, &QAction::triggered, this, &MainWindow::showBlockchainChooser);
    connect(words, &QAction::triggered, this, &MainWindow::showRecoveryWords);
    connect(bcdone, &QPushButton::clicked, &ot, &OTWrap::checkAccounts);
    connect(license, &QAction::triggered, this, &MainWindow::showLicenseViewer);
    connect(toolbox, &QToolBox::currentChanged, this, &MainWindow::changeChain);
    connect(this, &MainWindow::progMaxUpdated, prog, &QProgressBar::setMaximum);
    connect(this, &MainWindow::progValueUpdated, prog, &QProgressBar::setValue);
    showBlockchainStatistics();
}

void MainWindow::accountListUpdated(
    const QItemSelection& current,
    [[maybe_unused]] const QItemSelection& previous)
{
    const auto indices = current.indexes();

    if (0 == indices.size()) {
        showBlockchainStatistics();
    } else {
        auto& accountList = *imp_.ui_->accountList;
        const auto& index = indices.first();
        const auto& model = *accountList.model();
        const auto accountID =
            model.data(index, OTModel::Roles::AccountIDRole).toString();
        showAccountActivity(accountID);
    }

    imp_.updateProgress();
}

auto MainWindow::initModels() -> void { imp_.init_models(this); }

auto MainWindow::setProgressMax(int max) -> void { emit progMaxUpdated(max); }

auto MainWindow::setProgressValue(int value) -> void
{
    emit progValueUpdated(value);
}

auto MainWindow::showAccountActivity(int chain) -> void
{
    auto& accountActivity = *imp_.ui_->accountActivity;
    accountActivity.setModel(imp_.ot_.accountActivityModel(chain));
}

auto MainWindow::showAccountActivity(QString account) -> void
{
    auto& accountActivity = *imp_.ui_->accountActivity;
    accountActivity.setModel(imp_.ot_.accountActivityModel(account));
}

auto MainWindow::showBlockchainChooser() -> void
{
    util::Focuser(imp_.blockchains_.get()).show();
}

auto MainWindow::showBlockchainStatistics() -> void
{
    auto& accountActivity = *imp_.ui_->accountActivity;
    accountActivity.setModel(imp_.ot_.blockchainStatisticsModel());
}

auto MainWindow::showLicenseViewer() -> void
{
    util::Focuser(imp_.licenses_.get()).show();
}

auto MainWindow::showRecoveryWords() -> void
{
    auto dialog = std::make_unique<ShowSeed>(this, imp_.ot_);
    auto postcondition = ScopeGuard{[&]() { dialog.release(); }};
    util::Focuser(dialog.get()).show();
}

auto MainWindow::changeChain() -> void
{
    const auto chain = imp_.chain_toolbox_.currentChain();

    if (ot::blockchain::Type::Unknown == chain) {
        showBlockchainStatistics();
    } else {
        showAccountActivity(static_cast<int>(chain));
    }

    imp_.updateProgress();
}

auto MainWindow::updateToolbox() -> void
{
    imp_.chain_toolbox_.reconcile(imp_.ot_.enabledBlockchains());
}

MainWindow::~MainWindow() = default;
}  // namespace metier::widget
