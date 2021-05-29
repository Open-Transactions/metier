// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "blockchainsend.hpp"  // IWYU pragma: associated

#include <opentxs/ui/qt/AmountValidator.hpp>
#include <opentxs/ui/qt/DestinationValidator.hpp>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <cassert>

#include "ui_blockchainsend.h"
#include "widgets/blockchainsend/imp.hpp"

namespace metier::widget
{
BlockchainSend::BlockchainSend(QWidget* parent, Model* model)
    : QDialog(parent)
    , imp_p_(std::make_unique<Imp>(this, model))
    , imp_(*imp_p_)
{
    assert(imp_p_);

    auto& ui = *imp_.ui_;
    auto* cancel = ui.buttons->button(QDialogButtonBox::Cancel);
    auto* ok = ui.buttons->button(QDialogButtonBox::Ok);
    auto* address = ui.address;
    auto* amount = ui.amount;
    auto* vAddress = imp_.address_validator_;
    auto* vAmount = imp_.amount_validator_;
    auto* scale = ui.scale;
    connect(cancel, &QPushButton::clicked, this, &QWidget::close);
    connect(address, &QLineEdit::textChanged, [this]() { imp_.checkOk(); });
    connect(amount, &QLineEdit::textChanged, [this]() { imp_.checkOk(); });
    connect(ok, &QPushButton::clicked, [this]() { imp_.handle_ok(); });
    connect(
        scale,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index) { imp_.scaleChanged(index); });
    using AddressV = opentxs::ui::DestinationValidator;
    using AmountV = opentxs::ui::AmountValidator;
    connect(
        vAddress,
        &AddressV::detailsChanged,
        this,
        &BlockchainSend::updateAddress);
    connect(
        vAmount, &AmountV::scaleChanged, this, &BlockchainSend::updateAmount);
}

auto BlockchainSend::updateAddress(const QString& in) -> void
{
    imp_.updateStatus(in);
}

auto BlockchainSend::updateAmount(int in) -> void
{
    imp_.recalculateAmount(in);
}

auto BlockchainSend::updateSendResult(int key, int code, QString txid) -> void
{
    imp_.updateSendResult(key, code, txid);
}

BlockchainSend::~BlockchainSend()
{
    auto* vAddress = imp_.address_validator_;
    auto* vAmount = imp_.amount_validator_;
    using AddressV = opentxs::ui::DestinationValidator;
    using AmountV = opentxs::ui::AmountValidator;
    disconnect(
        vAddress,
        &AddressV::detailsChanged,
        this,
        &BlockchainSend::updateAddress);
    disconnect(
        vAmount, &AmountV::scaleChanged, this, &BlockchainSend::updateAmount);
}
}  // namespace metier::widget
