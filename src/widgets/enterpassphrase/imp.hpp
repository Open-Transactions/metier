// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/enterpassphrase.hpp"  // IWYU pragma: associated

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <cmath>
#include <mutex>
#include <string>

#include "ui_enterpassphrase.h"
#include "util/resizer.hpp"

namespace metier::widget
{
struct EnterPassphrase::Imp {
    std::unique_ptr<Ui::EnterPassphrase> ui_;

    auto passphraseChanged() noexcept -> void
    {
        auto lock = std::lock_guard<std::mutex>{lock_};
        auto* passphrase = ui_->passphrase;
        auto* retype = ui_->retype;
        passphrase_ = passphrase->text();
        retype_ = QString("");
        retype->setText(retype_);
        valid_retype_ =
            run_one_or_two_ ? true : (0 == retype_.compare(passphrase_));
        check_ok(lock);
    }
    auto retypeChanged() noexcept -> void
    {
        auto lock = std::lock_guard<std::mutex>{lock_};
        auto* passphrase = ui_->passphrase;
        auto* retype = ui_->retype;
        passphrase_ = passphrase->text();
        retype_ = retype->text();
        valid_retype_ =
            run_one_or_two_ ? true : (0 == retype_.compare(passphrase_));
        check_ok(lock);
    }
    auto cancel() noexcept -> void
    {
        auto lock = std::lock_guard<std::mutex>{lock_};
        canceled_ = true;
    }

    auto secret(opentxs::Secret& theSecret, QString& qstrSecret) -> bool
    {
        auto lock = std::lock_guard<std::mutex>{lock_};

        if (0 == passphrase_.size()) { passphrase_ = "opentxs"; }

        if (valid()) {
            qstrSecret = passphrase_;
            const std::string str_passphrase = passphrase_.toStdString();
            theSecret.Assign(str_passphrase.c_str(), str_passphrase.size());
            return true;
        }

        return false;
    }

    Imp(EnterPassphrase* parent,
        const QString& displayString,
        bool runOneOrTwo) noexcept
        : ui_(std::make_unique<Ui::EnterPassphrase>())
        , run_one_or_two_(runOneOrTwo)
        , lock_()
        , valid_retype_(true)
        , canceled_(false)
        , passphrase_("")
        , retype_("")
    {
        assert(ui_);

        auto& ui = *ui_;
        ui.setupUi(parent);

        if (displayString.size() > 0) {
            auto* display_label = ui.displayString;
            display_label->setText(displayString);
        }

        auto* passphrase = ui_->passphrase;
        auto* retype = ui_->retype;

        passphrase->setEchoMode(QLineEdit::Password);
        passphrase->setStyleSheet(
            QLatin1String("lineedit-password-character: 9679"));
        retype->setEchoMode(QLineEdit::Password);
        retype->setStyleSheet(
            QLatin1String("lineedit-password-character: 9679"));

        util::set_minimum_size(*passphrase, 55, 1);

        if (runOneOrTwo) {
            auto* retype_label = ui_->labelRetype;
            retype->hide();
            retype_label->hide();
        }

        check_ok();
    }

private:
    const bool run_one_or_two_;
    mutable std::mutex lock_;
    bool valid_retype_;
    bool canceled_;

    QString passphrase_;
    QString retype_;

    auto check_ok(const std::lock_guard<std::mutex>&) -> void
    {
        auto* ok_button = ui_->buttons->button(QDialogButtonBox::Ok);
        ok_button->setEnabled(valid_retype_);
    }
    auto check_ok() -> void
    {
        auto lock = std::lock_guard<std::mutex>{lock_};
        check_ok(lock);
    }
    auto valid() const -> bool { return valid_retype_ && !canceled_; }
};
}  // namespace metier::widget
