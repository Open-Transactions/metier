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

#include "models/accountactivity.hpp"
#include "otwrap.hpp"
#include "ui_mainwindow.h"
#include "util/resizer.hpp"
#include "util/scopeguard.hpp"
#include "widgets/blockchainchooser.hpp"
#include "widgets/licenses.hpp"
#include "widgets/mainwindow/chaintoolboxmanager.hpp"
#include "widgets/mainwindow/syncprogress.hpp"

namespace ot = opentxs;

namespace metier::widget
{
struct MainWindow::Imp {
    OTWrap& ot_;
    std::unique_ptr<Ui::MainWindow> ui_;
    std::unique_ptr<widget::BlockchainChooser> blockchains_;
    std::unique_ptr<widget::Licenses> licenses_;
    std::set<std::uintptr_t> registered_chains_;
    SyncProgress sync_progress_;
    ChainToolboxManager chain_toolbox_;

    auto init_models(MainWindow* parent) noexcept
    {
        auto& view = *ui_->accountList;
        view.setModel(ot_.accountListModel());
        auto* selection = view.selectionModel();
        connect(
            selection,
            &QItemSelectionModel::selectionChanged,
            parent,
            &MainWindow::accountListUpdated);
        updateProgress();
    }
    auto updateProgress() noexcept -> void
    {
        const auto chain = chain_toolbox_.currentChain();
        const auto [current, max] = sync_progress_.get(chain);
        ui_->syncProgress->setMaximum(max);
        ui_->syncProgress->setValue(current);
    }

    Imp(QMainWindow* parent, OTWrap& ot) noexcept
        : ot_(ot)
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
            *ui_->accountList, ot_.longestBlockchainName() + 4));
        {
            auto& accountActivity = *ui_->accountActivity;
            auto& header = *accountActivity.horizontalHeader();
            header.setSectionResizeMode(QHeaderView::ResizeToContents);
        }
    }

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
        auto* model = ot_.accountActivityModel(static_cast<int>(chain));

        assert(nullptr != model);

        if (auto ptr = reinterpret_cast<std::uintptr_t>(model);
            0 < registered_chains_.count(ptr)) {
            return;
        } else {
            registered_chains_.emplace(ptr);
        }

        connect(model, &model::AccountActivity::sync, [=](int value, int max) {
            receive_progress_update(chain, value, max);
        });
    }
};
}  // namespace metier::widget
