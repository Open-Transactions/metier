// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "blockchainchooser.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <ui_blockchainchooser.h>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTableView>
#include <iostream>

#include "metier/common/api.hpp"
#include "metier/skins/metier/util/resizer.hpp"

constexpr auto enabled_column_width_{10};

namespace metier::skins::metier::widget
{
BlockchainChooser::BlockchainChooser(QObject* parent, common::Api& ot)
    : QDialog(nullptr)
    , ot_(ot)
    , ui_(std::make_unique<Ui::BlockchainChooser>())
{
    moveToThread(parent->thread());
    ui_->setupUi(this);
    model();
    const auto longestBlockchainName = ot_.longestBlockchainName();

    {
        auto& widget = *ui_->mainnet;
        const auto setWidth = [&](auto& table) {
            const auto width =
                util::line_width(table, longestBlockchainName + 6);
            table.setColumnWidth(0, width);
        };
        setWidth(widget);
    }

    {
        const auto width = util::line_width(
            *this, longestBlockchainName + enabled_column_width_ + 12);
        const auto height = util::line_height(*this);
        setMinimumSize(width, height * 30);
    }

    auto* ok = ui_->buttons->button(QDialogButtonBox::Ok);
    auto* check = ui_->checkBox;
    connect(&ot_, &common::Api::chainsChanged, this, &BlockchainChooser::check);
    connect(&ot_, &common::Api::chainsChanged, this, &BlockchainChooser::check);
    connect(ok, &QPushButton::clicked, this, &BlockchainChooser::hide);
    connect(check, &QCheckBox::stateChanged, this, &BlockchainChooser::model);
    init();
}

auto BlockchainChooser::check(int enabledChains) noexcept -> void
{
    ui_->buttons->setEnabled(0 < enabledChains);
}

auto BlockchainChooser::init() noexcept -> void
{
    check(ot_.enabledCurrencyCount());
}

auto BlockchainChooser::Ok() noexcept -> QPushButton*
{
    return ui_->buttons->button(QDialogButtonBox::Ok);
}

auto BlockchainChooser::model() noexcept -> void
{
    auto& widget = *ui_->mainnet;
    auto& check = *ui_->checkBox;
    auto* model = ot_.blockchainChooserModel(check.isChecked());
    widget.setModel(model);
}

BlockchainChooser::~BlockchainChooser() = default;
}  // namespace metier::skins::metier::widget
