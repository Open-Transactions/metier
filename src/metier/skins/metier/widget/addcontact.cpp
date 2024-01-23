// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "addcontact.hpp"  // IWYU pragma: associated

#include <ui_addcontact.h>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QToolButton>

#include "metier/common/api.hpp"
#include "metier/skins/metier/util/resizer.hpp"

namespace metier::skins::metier::widget
{
AddContact::AddContact(QObject* parent, common::Api& ot)
    : QDialog(nullptr)
    , ui_(std::make_unique<Ui::AddContact>())
    , ot_(ot)
{
    moveToThread(parent->thread());
    ui_->setupUi(this);
    auto* label = ui_->contactLabelText;
    label->setMaximumHeight(util::line_height(*label, {3, 2}));
    util::set_minimum_size(*label, 10, 1, {3, 2});
    auto* id = ui_->paymentCodeText;
    id->setMaximumHeight(util::line_height(*id, {3, 2}));
    util::set_minimum_size(*id, 55, 1, {3, 2});
    auto* ok = ui_->buttonBox->button(QDialogButtonBox::Ok);
    auto* cancel = ui_->buttonBox->button(QDialogButtonBox::Cancel);
    connect(ok, &QPushButton::clicked, this, &AddContact::addContactPrivate);
    connect(cancel, &QPushButton::clicked, this, &QDialog::close);
}

auto AddContact::addContact(QString label, QString value) -> void
{
    ot_.addContact(std::move(label), std::move(value));
    close();
}

auto AddContact::addContactPrivate() -> void
{
    addContact(ui_->contactLabelText->text(), ui_->paymentCodeText->text());
}

AddContact::~AddContact() = default;
}  // namespace metier::skins::metier::widget
