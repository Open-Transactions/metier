// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "blockchainsend.hpp"  // IWYU pragma: associated

#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <cassert>

#include "ui_blockchainsend.h"
#include "widgets/blockchainsend/imp.hpp"

namespace metier::widget
{
BlockchainSend::BlockchainSend(QWidget* parent, model::AccountActivity* model)
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
    connect(cancel, &QPushButton::clicked, this, &QWidget::close);
    connect(
        address, &QLineEdit::textChanged, [this]() { imp_.addressChanged(); });
    connect(
        amount, &QLineEdit::textChanged, [this]() { imp_.amountChanged(); });
    connect(ok, &QPushButton::clicked, [this]() {
        imp_.send();
        close();
    });
}

BlockchainSend::~BlockchainSend() = default;
}  // namespace metier::widget
