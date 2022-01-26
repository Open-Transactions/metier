// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/mainwindow.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <deque>
#include <iostream>
#include <set>
#include <tuple>

#include "api/api.hpp"
#include "ui_mainwindow.h"
#include "util/resizer.hpp"
#include "util/scopeguard.hpp"
#include "widgets/addcontact.hpp"
#include "widgets/blockchainchooser.hpp"
#include "widgets/licenses.hpp"
#include "widgets/mainwindow/chaintoolboxmanager.hpp"
#include "widgets/mainwindow/syncprogress.hpp"

namespace ot = opentxs;

namespace metier::widget
{
struct MainWindow::Imp {
    MainWindow& parent_;
    Api& ot_;
    opentxs::ui::IdentityManagerQt* identity_manager_;
    std::unique_ptr<Ui::MainWindow> ui_;
    std::unique_ptr<widget::BlockchainChooser> blockchains_;
    std::unique_ptr<widget::Licenses> licenses_;
    std::set<std::uintptr_t> registered_chains_;
    SyncProgress sync_progress_;
    ChainToolboxManager chain_toolbox_;

    auto init_models(MainWindow* parent) noexcept
    {
        {
            auto& view = *ui_->accountList;
            view.setModel(identity_manager_->getAccountList());
            auto* selection = view.selectionModel();
            connect(
                selection,
                &QItemSelectionModel::selectionChanged,
                parent,
                &MainWindow::accountListUpdated);
        }

        {
            auto& view = *ui_->contactListView;
            view.setModel(identity_manager_->getContactList());
            auto* selection = view.selectionModel();
            connect(
                selection,
                &QItemSelectionModel::selectionChanged,
                parent,
                &MainWindow::contactListUpdated);
        }

        using Profile = opentxs::ui::ProfileQt;
        auto* profile = identity_manager_->getProfile();
        connect(
            profile,
            &Profile::displayNameChanged,
            parent,
            &MainWindow::updateName);
        connect(
            profile,
            &Profile::paymentCodeChanged,
            parent,
            &MainWindow::updatePaymentCode);
        updateName(profile->displayName());
        updatePaymentCode(profile->paymentCode());
        updateProgress();
    }
    auto showAddContact() noexcept -> void
    {
        auto dialog = std::make_unique<AddContact>(&parent_, ot_);
        auto postcondition = ScopeGuard{[&dialog]() {
            dialog->deleteLater();
            dialog.release();
        }};
        dialog->exec();
    }
    auto updateName(QString value) noexcept -> void
    {
        auto& name = *ui_->profileName;
        name.setText(value);
    }
    auto updatePaymentCode(QString value) -> void
    {
        auto& code = *ui_->paymentCode;
        auto& qr = *ui_->qrCode;
        code.setText(value);
        qr.setString(value);
    }
    auto updateProgress() noexcept -> void
    {
        const auto chain = chain_toolbox_.currentChain();
        const auto [current, max] = sync_progress_.get(chain);
        parent_.setProgressMax(max);
        parent_.setProgressValue(current);
    }

    Imp(MainWindow* parent, Api& ot) noexcept
        : parent_(*parent)
        , ot_(ot)
        , identity_manager_(ot_.identityManager())
        , ui_(std::make_unique<Ui::MainWindow>())
        , blockchains_(std::make_unique<widget::BlockchainChooser>(parent, ot_))
        , licenses_(std::make_unique<widget::Licenses>(parent))
        , registered_chains_()
        , sync_progress_()
        , chain_toolbox_(
              parent,
              ot_,
              ui_->accountList,
              ui_->moneyToolbox,
              [this](const auto chain) { register_progress(chain); })
    {
        ui_->setupUi(parent);
        ui_->moneyToolbox->setMaximumWidth(util::line_width(
            *ui_->accountList, ot_.longestBlockchainName() + 16));
        {
            auto& accountActivity = *ui_->accountActivity;
            auto& header = *accountActivity.horizontalHeader();
            header.setSectionResizeMode(QHeaderView::ResizeToContents);
        }

        ui_->header->setMinimumHeight(138);
        ui_->header->setMaximumHeight(138);
        auto& paymentCode = *ui_->paymentCode;
        util::set_minimum_size(paymentCode, 72, 1);
        ui_->identity->setMaximumWidth(paymentCode.minimumWidth());
        paymentCode.setStyleSheet("* { background-color: rgba(0, 0, 0, 0); }");

        {
            {
                auto& activityThread = *ui_->activityThreadView;
                auto& header = *activityThread.horizontalHeader();
                header.setSectionResizeMode(QHeaderView::ResizeToContents);
            }

            ui_->contactListFrame->setMaximumWidth(util::line_width(
                *ui_->contactListView, ot_.longestBlockchainName() + 16));
            ui_->messageEdit->setMaximumHeight(
                util::line_height(*ui_->contactListView, {3, 2}) * 2);
            ui_->messageEdit->setEnabled(false);
            ui_->sendMessage->setEnabled(false);
            connect(
                ui_->addContact,
                &QPushButton::clicked,
                &parent_,
                &MainWindow::showAddContact);
        }

        connect(&parent_, &QObject::destroyed, &ot_, &Api::quit);
    }

    Imp() = delete;
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;

private:
    auto receive_progress_update(
        const ot::blockchain::Type chain,
        const int value,
        const int max) noexcept
    {
        sync_progress_.update(chain, {value, max});
        updateProgress();
    }
    auto register_progress(const ot::blockchain::Type chain) noexcept -> void
    {
        const auto accountID =
            ot_.blockchainTypeToAccountID(static_cast<int>(chain));
        auto* model = identity_manager_->getAccountActivity(accountID);

        assert(nullptr != model);

        if (auto ptr = reinterpret_cast<std::uintptr_t>(model);
            0 < registered_chains_.count(ptr)) {
            return;
        } else {
            registered_chains_.emplace(ptr);
        }

        using Model = ot::ui::AccountActivityQt;
        connect(model, &Model::syncProgressUpdated, [=](int value, int max) {
            receive_progress_update(chain, value, max);
        });
    }
};
}  // namespace metier::widget
