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
    bool runOneOrTwo)
    : QDialog(parent)
    , imp_p_(std::make_unique<Imp>(this, displayString, runOneOrTwo))
    , imp_(*imp_p_)
{
    assert(imp_p_);

    auto& ui = *imp_.ui_;
    auto* cancel = ui.buttons->button(QDialogButtonBox::Cancel);
    auto* passphrase = ui.passphrase;
    auto* retype = ui.retype;
    connect(cancel, &QPushButton::clicked, [this]() {
        imp_.cancel();
        close();
    });
    connect(passphrase, &QLineEdit::textChanged, [this]() {
        imp_.passphraseChanged();
    });
    connect(
        retype, &QLineEdit::textChanged, [this]() { imp_.retypeChanged(); });
}

auto EnterPassphrase::secret() -> QString { return imp_.secret(); }

EnterPassphrase::~EnterPassphrase() = default;
}  // namespace metier::widget
