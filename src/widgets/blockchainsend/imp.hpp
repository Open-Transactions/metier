// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/blockchainsend.hpp"  // IWYU pragma: associated

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <cmath>
#include <mutex>
#include <string>

#include "models/accountactivity.hpp"
#include "ui_blockchainsend.h"
#include "util/resizer.hpp"

namespace metier::widget
{
struct BlockchainSend::Imp {
    std::unique_ptr<Ui::BlockchainSend> ui_;

    auto addressChanged() noexcept -> void
    {
        auto lock = std::lock_guard<std::mutex>{lock_};
        auto* address = ui_->address;
        valid_address_ = model_.validateAddress(address->text());
        check_ok(lock);
    }
    auto amountChanged() noexcept -> void
    {
        auto lock = std::lock_guard<std::mutex>{lock_};
        auto* amount = ui_->amount;
        formatted_amount_ = model_.validateAmount(amount->text());
        valid_amount_ = 0 < formatted_amount_.size();
        check_ok(lock);
    }
    auto send() noexcept -> void
    {
        auto* address = ui_->address;
        auto* amount = ui_->amount;
        auto* memo = ui_->memo;
        model_.sendToAddress(address->text(), amount->text(), memo->text());
    }

    Imp(BlockchainSend* parent, model::AccountActivity* model) noexcept
        : ui_(std::make_unique<Ui::BlockchainSend>())
        , lock_()
        , model_(*model)
        , formatted_amount_()
        , valid_address_(false)
        , valid_amount_(false)
    {
        assert(ui_);
        assert(nullptr != model);

        auto& ui = *ui_;
        ui.setupUi(parent);
        auto* ok = ui.buttons->button(QDialogButtonBox::Ok);
        auto* address = ui.address;
        ok->setText(tr("Pay"));
        util::set_minimum_size(*address, 72, 1);
        check_ok();
    }

private:
    mutable std::mutex lock_;
    model::AccountActivity& model_;
    QString formatted_amount_;
    bool valid_address_;
    bool valid_amount_;

    auto check_ok(const std::lock_guard<std::mutex>&) -> void
    {
        auto* ok = ui_->buttons->button(QDialogButtonBox::Ok);
        ok->setEnabled(valid_address_ && valid_amount_);
    }
    auto check_ok() -> void
    {
        auto lock = std::lock_guard<std::mutex>{lock_};
        check_ok(lock);
    }
};
}  // namespace metier::widget
