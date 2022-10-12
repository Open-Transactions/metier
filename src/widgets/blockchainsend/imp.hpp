// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/blockchainsend.hpp"  // IWYU pragma: associated

#include <opentxs/Qt.hpp>
#include <opentxs/opentxs.hpp>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <cmath>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>

#include "ui_blockchainsend.h"
#include "util/resizer.hpp"

namespace ot = opentxs;

namespace metier::widget
{
struct BlockchainSend::Imp {
    BlockchainSend& parent_;
    std::unique_ptr<Ui::BlockchainSend> ui_;
    Model& model_;
    opentxs::ui::AmountValidator* amount_validator_;
    opentxs::ui::DestinationValidator* address_validator_;
    std::mutex lock_;
    std::optional<int> key_;
    bool finished_;

    auto checkOk() -> void
    {
        auto* ok = ui_->buttons->button(QDialogButtonBox::Ok);
        auto* address = ui_->address;
        auto* amount = ui_->amount;
        ok->setEnabled(
            address->hasAcceptableInput() && amount->hasAcceptableInput());
    }
    auto fail() noexcept -> void
    {
        lock_input();
        auto* result = ui_->result;
        result->setText("Unspecified fatal error");
        finished();
    }
    auto finished() noexcept -> void
    {
        finished_ = true;
        auto* ok = ui_->buttons->button(QDialogButtonBox::Ok);
        ok->setText("Done");
        ok->setEnabled(true);
    }
    auto handle_ok() noexcept -> void
    {
        if (finished_) {
            parent_.close();
        } else {
            send();
        }
    }
    auto lock_input() noexcept -> void
    {
        auto* address = ui_->address;
        auto* amount = ui_->amount;
        auto* memo = ui_->memo;
        auto* scale = ui_->scale;
        auto* ok = ui_->buttons->button(QDialogButtonBox::Ok);
        auto* cancel = ui_->buttons->button(QDialogButtonBox::Cancel);
        address->setReadOnly(true);
        amount->setReadOnly(true);
        scale->setEditable(false);
        memo->setReadOnly(true);
        ok->setEnabled(false);
        cancel->setEnabled(false);
    }
    auto recalculateAmount(int scale) noexcept
    {
        auto* amount = ui_->amount;
        auto text = amount->text();
        amount->setText(amount_validator_->revise(text, scale));
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
        auto failed{false};

        {
            auto lock = std::lock_guard<std::mutex>(lock_);
            key_ = model_.sendToAddress(
                address->text(),
                amount->text(),
                memo->text(),
                amount_validator_->getScale());
            failed = (0 > key_.value());
        }

        if (failed) {
            fail();
        } else {
            wait();
        }
    }
    auto updateSendResult(int key, int code, const QString& txid) -> void
    {
        auto lock = std::lock_guard<std::mutex>(lock_);

        if (false == key_.has_value()) { return; }
        if (key_.value() != key) { return; }

        using Result = ot::blockchain::node::SendResult;
        const auto result = static_cast<Result>(code);
        auto status = QString::fromStdString(std::string{print(result)});

        if (0 < txid.size()) { status.append(" ").append(txid); }

        ui_->result->setText(status);
        finished();
    }
    auto updateStatus(const QString& text) noexcept -> void
    {
        auto* status = ui_->status;
        status->setText(text);
    }
    auto wait() noexcept -> void { lock_input(); }

    Imp(BlockchainSend* parent, Model* model) noexcept
        : parent_(*parent)
        , ui_(std::make_unique<Ui::BlockchainSend>())
        , model_(*model)
        , amount_validator_(model_.getAmountValidator())
        , address_validator_(model_.getDestValidator())
        , lock_()
        , key_(std::nullopt)
        , finished_(false)
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
        parent->connect(
            model,
            &Model::transactionSendResult,
            parent,
            &BlockchainSend::updateSendResult);
        checkOk();
    }

    Imp() = delete;
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;
};
}  // namespace metier::widget
