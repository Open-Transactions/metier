// Copyright (c) 2019-2022 The Open-Transactions developers
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

    static auto factory(
        EnterPassphrase* parent,
        const QString& prompt,
        Mode mode) noexcept -> std::unique_ptr<Imp>;

    auto secret() const noexcept -> QString;

    auto cancel() noexcept -> void;
    virtual auto confirmChanged() noexcept -> void = 0;
    virtual auto passphraseChanged() noexcept -> void = 0;

    virtual ~Imp() = default;

protected:
    mutable std::mutex lock_;
    const QString prompt_;
    QString entry_;

    auto get_confirm() const noexcept -> QString;
    auto get_entry() const noexcept -> QString;
    virtual auto validate() const noexcept -> bool = 0;

    auto disable_ok() noexcept -> void;
    auto enable_ok() noexcept -> void;
    auto hide_confirm() noexcept -> void;
    auto use_default_password() noexcept -> void;
    auto use_provided_password() noexcept -> void;
    auto show_confirm() noexcept -> void;

    Imp(EnterPassphrase* parent, const QString& prompt) noexcept;

private:
    auto default_password() const noexcept -> const QString&;
};
}  // namespace metier::widget
