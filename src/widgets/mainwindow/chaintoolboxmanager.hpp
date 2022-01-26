// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/mainwindow.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QTableView>
#include <deque>
#include <functional>
#include <iostream>
#include <tuple>

#include "api/api.hpp"
#include "ui_mainwindow.h"
#include "util/convertblockchain.hpp"
#include "util/resizer.hpp"
#include "util/scopeguard.hpp"
#include "widgets/accountstatus.hpp"
#include "widgets/blockchainchooser.hpp"
#include "widgets/blockchainsend.hpp"
#include "widgets/licenses.hpp"
#include "widgets/receivingaddress.hpp"

namespace ot = opentxs;

namespace metier::widget
{
constexpr auto dummyIndex{-88};

struct ChainToolboxManager {
    using AccountList = QListView*;
    using ToolBox = QToolBox*;
    using RegisterProgress = std::function<void(const ot::blockchain::Type)>;

    auto currentChain() const noexcept
    {
        ot::rLock lock{lock_};
        const auto index = toolbox_->currentIndex();

        if (0 > index) { return ot::blockchain::Type::Unknown; }

        const auto position = static_cast<std::size_t>(index);

        assert(position < items_.size());

        if (0 == position) {
            assert(nullptr != account_list_);

            auto& accountList = *account_list_;
            auto& selection = *accountList.selectionModel();
            selection.clear();

            return ot::blockchain::Type::Unknown;
        }

        return util::convert(items_.at(position));
    }

    auto reconcile(Api::BlockchainList current) noexcept
    {
        ot::rLock lock{lock_};
        std::sort(std::begin(current), std::end(current));
        auto itemsToDelete = std::deque<int>{};
        auto itemsToInsert = std::deque<int>{};
        // skip the first item that represents the overview tab
        const auto effectiveStart = std::next(std::begin(items_));
        std::set_difference(
            effectiveStart,
            std::end(items_),
            std::begin(current),
            std::end(current),
            std::back_inserter(itemsToDelete));
        std::set_difference(
            std::begin(current),
            std::end(current),
            effectiveStart,
            std::end(items_),
            std::back_inserter(itemsToInsert));

        if (0 < itemsToDelete.size()) {
            for (auto i{items_.begin()}; i < items_.end();) {
                if (*i == itemsToDelete.front()) {
                    const auto index =
                        static_cast<int>(std::distance(items_.begin(), i));
                    delete_tab(index);
                    i = items_.erase(i);
                    itemsToDelete.pop_front();

                    if (0 == itemsToDelete.size()) { break; }
                } else {
                    ++i;
                }
            }
        }

        assert(0 == itemsToDelete.size());

        if (0 < itemsToInsert.size()) {
            for (auto i{items_.begin()}; i < items_.end();) {
                if (const auto chain = itemsToInsert.front(); *i > chain) {
                    i = items_.emplace(i, chain);
                    const auto index =
                        static_cast<int>(std::distance(items_.begin(), i));
                    create_tab(index, chain);
                    itemsToInsert.pop_front();

                    if (0 == itemsToInsert.size()) { break; }
                } else {
                    ++i;
                }
            }
        }

        for (const auto& chain : itemsToInsert) {
            auto i = items_.emplace(items_.end(), chain);
            const auto index =
                static_cast<int>(std::distance(items_.begin(), i));
            create_tab(index, chain);
        }
    }

    ChainToolboxManager(
        QMainWindow* parent,
        Api& ot,
        AccountList& accountList,
        ToolBox& toolbox,
        RegisterProgress progress) noexcept
        : parent_(parent)
        , lock_()
        , progress_cb_(progress)
        , ot_(ot)
        , identity_manager_(ot_.identityManager())
        , account_list_(accountList)
        , toolbox_(toolbox)
        , items_({dummyIndex})  // NOTE: dummy index represents overview tab
    {
        assert(progress);
    }

private:
    QMainWindow* parent_;
    mutable std::recursive_mutex lock_;
    const RegisterProgress progress_cb_;
    Api& ot_;
    opentxs::ui::IdentityManagerQt* identity_manager_;
    AccountList& account_list_;
    ToolBox& toolbox_;
    std::vector<int> items_;

    auto create_tab(const int position, const int chain) noexcept -> void
    {
        assert(nullptr != toolbox_);

        auto [widget, description] = make_widget(util::convert(chain));
        toolbox_->insertItem(
            position, widget.release(), QString::fromUtf8(description.c_str()));
    }
    auto delete_tab(const int position) noexcept -> void
    {
        auto* widget = toolbox_->widget(position);

        assert(nullptr != widget);

        toolbox_->removeItem(position);
        widget->deleteLater();
    }
    auto make_widget(const ot::blockchain::Type chain) noexcept
        -> std::pair<std::unique_ptr<QWidget>, std::string>
    {
        auto output = std::make_pair<std::unique_ptr<QWidget>, std::string>(
            std::make_unique<QWidget>(toolbox_),
            ot::blockchain::DisplayString(chain));
        auto& [widget, description] = output;
        auto layout = std::make_unique<QGridLayout>(widget.get());
        auto view = std::make_unique<QTableView>(widget.get());
        auto receive = std::make_unique<QPushButton>(widget.get());
        auto send = std::make_unique<QPushButton>(widget.get());
        auto details = std::make_unique<QPushButton>(widget.get());
        auto postcondition = ScopeGuard{[&]() {
            layout.release();
            view.release();
            receive.release();
            send.release();
            details.release();
        }};
        const auto widgetName = ot::blockchain::TickerSymbol(chain) + "Tab";
        const auto layoutName = widgetName + "Layout";
        const auto viewName = widgetName + "AccountSummary";
        const auto receiveName = widgetName + "Receive";
        const auto sendName = widgetName + "Send";
        const auto detailsName = widgetName + "Details";
        widget->setObjectName(QString::fromUtf8(widgetName.c_str()));
        layout->setObjectName(QString::fromUtf8(layoutName.c_str()));
        view->setObjectName(QString::fromUtf8(viewName.c_str()));
        receive->setObjectName(QString::fromUtf8(receiveName.c_str()));
        receive->setText(
            QCoreApplication::translate("MainWindow", "Receive", nullptr));
        receive->connect(receive.get(), &QPushButton::clicked, [=]() {
            show_receiving(chain);
        });
        send->setObjectName(QString::fromUtf8(sendName.c_str()));
        send->setText(
            QCoreApplication::translate("MainWindow", "Send", nullptr));
        send->connect(
            send.get(), &QPushButton::clicked, [=]() { show_send(chain); });
        details->setObjectName(QString::fromUtf8(detailsName.c_str()));
        details->setText(
            QCoreApplication::translate("MainWindow", "Details", nullptr));
        details->connect(details.get(), &QPushButton::clicked, [=]() {
            show_details(chain);
        });
        layout->addWidget(view.get(), 0, 0, 1, 2);
        layout->addWidget(receive.get(), 1, 0, 1, 1);
        layout->addWidget(send.get(), 1, 1, 1, 1);
        layout->addWidget(details.get(), 2, 1, 1, 1);
        progress_cb_(chain);

        return output;
    }
    auto show_details(const ot::blockchain::Type chain) noexcept -> void
    {
        auto dialog = std::make_unique<AccountStatus>(
            parent_, ot_, static_cast<int>(chain));
        auto postcondition = ScopeGuard{[&dialog]() {
            dialog->deleteLater();
            dialog.release();
        }};
        dialog->exec();
    }
    auto show_receiving(const ot::blockchain::Type chain) noexcept -> void
    {
        const auto accountID =
            ot_.blockchainTypeToAccountID(static_cast<int>(chain));
        auto* model = identity_manager_->getAccountActivity(accountID);

        if (nullptr == model) { return; }

        auto dialog = std::make_unique<ReceivingAddress>(
            parent_, model->getDepositAddress());
        auto postcondition = ScopeGuard{[&dialog]() {
            dialog->deleteLater();
            dialog.release();
        }};
        dialog->exec();
    }
    auto show_send(const ot::blockchain::Type chain) noexcept -> void
    {
        const auto accountID =
            ot_.blockchainTypeToAccountID(static_cast<int>(chain));
        auto* model = identity_manager_->getAccountActivity(accountID);

        if (nullptr == model) { return; }

        auto dialog = std::make_unique<BlockchainSend>(parent_, model);
        auto postcondition = ScopeGuard{[&dialog]() {
            dialog->deleteLater();
            dialog.release();
        }};
        dialog->exec();
    }

    ChainToolboxManager() = delete;
    ChainToolboxManager(const ChainToolboxManager&) = delete;
    ChainToolboxManager(ChainToolboxManager&&) = delete;
    auto operator=(const ChainToolboxManager&) -> ChainToolboxManager& = delete;
    auto operator=(ChainToolboxManager&&) -> ChainToolboxManager& = delete;
};
}  // namespace metier::widget
