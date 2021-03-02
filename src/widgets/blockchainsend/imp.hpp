// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/blockchainsend.hpp"  // IWYU pragma: associated

#include <opentxs/ui/qt/AmountValidator.hpp>
#include <opentxs/ui/qt/DestinationValidator.hpp>
#include <opentxs/ui/qt/DisplayScale.hpp>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <cmath>
#include <iostream>
#include <mutex>
#include <string>

#include "models/accountactivity.hpp"
#include "ui_blockchainsend.h"
#include "util/resizer.hpp"

namespace metier::widget
{
struct BlockchainSend::Imp {
    std::unique_ptr<Ui::BlockchainSend> ui_;
    model::AccountActivity& model_;
    opentxs::ui::AmountValidator* amount_validator_;
    opentxs::ui::DestinationValidator* address_validator_;

    auto checkOk() -> void
    {
        auto* ok = ui_->buttons->button(QDialogButtonBox::Ok);
        auto* address = ui_->address;
        auto* amount = ui_->amount;
        ok->setEnabled(
            address->hasAcceptableInput() && amount->hasAcceptableInput());
    }
    auto recalculateAmount(int scale) noexcept
    {
        auto* amount = ui_->amount;
        amount->setText(amount_validator_->revise(amount->text(), scale));
    }
    auto scaleChanged(int scale) noexcept
    {
        amount_validator_->setScale(scale);
    }
    auto send() noexcept -> void
    {
        auto* address = ui_->address;
        auto* amount = ui_->amount;
        auto* memo = ui_->memo;
        model_.sendToAddress(address->text(), amount->text(), memo->text());
    }
    auto updateStatus(const QString& text) noexcept -> void
    {
        auto* status = ui_->status;
        status->setText(text);
    }

    Imp(BlockchainSend* parent, model::AccountActivity* model) noexcept
        : ui_(std::make_unique<Ui::BlockchainSend>())
        , model_(*model)
        , amount_validator_(model_.getAmountValidator())
        , address_validator_(model_.getDestValidator())
    {
        assert(ui_);
        assert(nullptr != model);

        auto& ui = *ui_;
        ui.setupUi(parent);
        auto* ok = ui.buttons->button(QDialogButtonBox::Ok);
        auto* address = ui.address;
        ok->setText(tr("Pay"));
        address->setValidator(address_validator_);
        util::set_minimum_size(*address, 72, 1);
        auto* amount = ui_->amount;
        amount->setValidator(amount_validator_);
        auto* scale = ui_->scale;
        scale->setModel(model_.getScaleModel());
        amount_validator_->setScale(scale->currentIndex());
        checkOk();
    }

    Imp() = delete;
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;
};
}  // namespace metier::widget
