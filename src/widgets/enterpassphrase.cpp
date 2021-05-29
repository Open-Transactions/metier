// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "enterpassphrase.hpp"  // IWYU pragma: associated

#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <cassert>

#include "ui_enterpassphrase.h"
#include "widgets/enterpassphrase/imp.hpp"

namespace metier::widget
{
EnterPassphrase::EnterPassphrase(
    QWidget* parent,
    const QString& displayString,
    Mode mode) noexcept
    : QDialog(parent)
    , imp_p_(Imp::factory(this, displayString, mode))
    , imp_(*imp_p_)
{
    assert(imp_p_);

    auto& ui = *imp_.ui_;
    auto* passphrase = ui.passphrase;
    auto* confirm = ui.retype;
    auto* cancel = ui.buttons->button(QDialogButtonBox::Cancel);
    auto* ok = ui.buttons->button(QDialogButtonBox::Ok);

    connect(cancel, &QPushButton::clicked, [this]() {
        imp_.cancel();
        close();
    });
    connect(ok, &QPushButton::clicked, [this]() { close(); });
    connect(passphrase, &QLineEdit::textChanged, [this]() {
        imp_.passphraseChanged();
    });
    connect(
        confirm, &QLineEdit::textChanged, [this]() { imp_.confirmChanged(); });
}

auto EnterPassphrase::secret() const noexcept -> QString
{
    return imp_.secret();
}

EnterPassphrase::~EnterPassphrase() = default;
}  // namespace metier::widget
