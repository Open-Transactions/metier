// Copyright (c) 2019-2020 The Open-Transactions developers
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

#include "models/accountlist.hpp"
#include "models/profile.hpp"
#include "otwrap.hpp"
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
    OTWrap& ot_;
    std::unique_ptr<Ui::MainWindow> ui_;
    std::unique_ptr<widget::BlockchainChooser> blockchains_;
    std::unique_ptr<widget::Licenses> licenses_;
    std::set<std::uintptr_t> registered_chains_;
    SyncProgress sync_progress_;
    ChainToolboxManager chain_toolbox_;
    model::Profile* profile_;

    auto init_models(MainWindow* parent) noexcept
    {
        {
            auto& view = *ui_->accountList;
            view.setModel(ot_.accountListModel());
            auto* selection = view.selectionModel();
            connect(
                selection,
                &QItemSelectionModel::selectionChanged,
                parent,
                &MainWindow::accountListUpdated);
        }

        {
            auto& view = *ui_->contactListView;
            view.setModel(ot_.contactListModel());
            auto* selection = view.selectionModel();
            connect(
                selection,
                &QItemSelectionModel::selectionChanged,
                parent,
                &MainWindow::contactListUpdated);
        }

        profile_ = ot_.profileModel();
        connect(profile_, &model::Profile::updated, [&] { repaintProfile(); });
        repaintProfile();
        updateProgress();
    }
    auto repaintProfile() noexcept -> void
    {
        OT_ASSERT(nullptr != profile_);

        auto& name = *ui_->profileName;
        auto& code = *ui_->paymentCode;
        auto& qr = *ui_->qrCode;
        name.setText(profile_->displayName());
        code.setText(profile_->paymentCode());
        qr.setString(profile_->paymentCode());
    }
    auto updateProgress() noexcept -> void
    {
        const auto chain = chain_toolbox_.currentChain();
        const auto [current, max] = sync_progress_.get(chain);
        parent_.setProgressMax(max);
        parent_.setProgressValue(current);
    }

    Imp(MainWindow* parent, OTWrap& ot) noexcept
        : parent_(*parent)
        , ot_(ot)
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
        , profile_()
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
            auto* addContact = ui_->addContact;
            addContact->connect(addContact, &QPushButton::clicked, [=]() {
                show_add_contact();
            });
        }
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
        using Model = OTWrap::AccountActivity;
        auto* model = ot_.accountActivityModel(static_cast<int>(chain));

        assert(nullptr != model);

        if (auto ptr = reinterpret_cast<std::uintptr_t>(model);
            0 < registered_chains_.count(ptr)) {
            return;
        } else {
            registered_chains_.emplace(ptr);
        }

        connect(model, &Model::syncProgressUpdated, [=](int value, int max) {
            receive_progress_update(chain, value, max);
        });
    }
    auto show_add_contact() noexcept -> void
    {
        auto dialog = std::make_unique<AddContact>(ot_);
        auto postcondition = ScopeGuard{[&dialog]() {
            dialog->deleteLater();
            dialog.release();
        }};
        dialog->exec();
    }
};
}  // namespace metier::widget
