// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/skins/metier/widget/profilealias.hpp"  // IWYU pragma: associated

#include <ui_profilealias.h>
#include <QObject>
#include <QPlainTextEdit>
#include <QToolButton>

#include "metier/skins/metier/util/resizer.hpp"

namespace metier::skins::metier::widget
{
ProfileAlias::ProfileAlias(QObject* parent)
    : QDialog(nullptr)
    , ui_(std::make_unique<Ui::ProfileAlias>())
{
    moveToThread(parent->thread());
    ui_->setupUi(this);
    auto* alias = ui_->alias;
    alias->setMaximumHeight(util::line_height(*alias, {3, 2}));
    util::set_minimum_size(*alias, 10, 1, {3, 2});
    connect(alias, &QPlainTextEdit::textChanged, this, &ProfileAlias::check);
    connect(ui_->confirm, &QToolButton::clicked, this, &ProfileAlias::confirm);
    connect(this, &ProfileAlias::gotAlias, this, &ProfileAlias::hide);
}

auto ProfileAlias::check() noexcept -> void
{
    auto& alias = *ui_->alias;
    auto& confirm = *ui_->confirm;
    auto text = alias.toPlainText();
    confirm.setEnabled(!text.isEmpty());
    auto done{false};

    if (text.contains('\n')) {
        text.remove('\n');
        done = true;
    }

    if (done) { this->confirm(); }
}

auto ProfileAlias::confirm() noexcept -> void
{
    Q_EMIT gotAlias(ui_->alias->toPlainText().remove('\n'));
}

ProfileAlias::~ProfileAlias() = default;
}  // namespace metier::skins::metier::widget
