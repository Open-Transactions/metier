// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "blockchainchooser.hpp"  // IWYU pragma: associated

#include <QDialogButtonBox>
#include <QPushButton>
#include <QTableView>

#include "otwrap.hpp"
#include "ui_blockchainchooser.h"
#include "util/resizer.hpp"

constexpr auto enabled_column_width_{10};

namespace metier::widget
{
BlockchainChooser::BlockchainChooser(QWidget* parent, OTWrap& ot)
    : QDialog(parent)
    , ot_(ot)
    , ui_(std::make_unique<Ui::BlockchainChooser>())
{
    ui_->setupUi(this);
    const auto longestBlockchainName = ot_.longestBlockchainName();

    {
        auto& mainnet = *ui_->mainnet;
        auto& testnet = *ui_->testnet;
        auto* mainModel = ot_.blockchainChooserModel(false);
        auto* testModel = ot_.blockchainChooserModel(true);
        mainnet.setModel(mainModel);
        testnet.setModel(testModel);
        const auto setWidth = [&](auto& table) {
            const auto width0 = util::line_width(table, longestBlockchainName);
            const auto width1 = util::line_width(table, enabled_column_width_);
            table.setColumnWidth(0, width0);
            table.setColumnWidth(1, width1);
        };
        setWidth(mainnet);
        setWidth(testnet);
    }

    {
        const auto width = util::line_width(
            *this, longestBlockchainName + enabled_column_width_ + 12);
        const auto height = util::line_height(*this);
        setMinimumSize(width, height * 30);
    }

    auto* ok = ui_->buttons->button(QDialogButtonBox::Ok);
    connect(&ot_, &OTWrap::chainsChanged, this, &BlockchainChooser::check);
    connect(&ot_, &OTWrap::chainsChanged, this, &BlockchainChooser::check);
    connect(ok, &QPushButton::clicked, [this]() { hide(); });
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

BlockchainChooser::~BlockchainChooser() = default;
}  // namespace metier::widget
