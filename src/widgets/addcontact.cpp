// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "addcontact.hpp"  // IWYU pragma: associated

#include <QPlainTextEdit>
#include <QPushButton>
#include <QToolButton>

#include "otwrap.hpp"
#include "ui_addcontact.h"
#include "util/resizer.hpp"

namespace metier::widget
{
AddContact::AddContact(OTWrap& ot)
    : QDialog(nullptr)
    , ui_(std::make_unique<Ui::AddContact>())
    , ot_(ot)
{
    ui_->setupUi(this);
    auto* label = ui_->contactLabelText;
    label->setMaximumHeight(util::line_height(*label, {3, 2}));
    util::set_minimum_size(*label, 10, 1, {3, 2});
    auto* id = ui_->paymentCodeText;
    id->setMaximumHeight(util::line_height(*id, {3, 2}));
    util::set_minimum_size(*id, 55, 1, {3, 2});
    auto* ok = ui_->buttonBox->button(QDialogButtonBox::Ok);
    auto* cancel = ui_->buttonBox->button(QDialogButtonBox::Cancel);
    connect(ok, &QPushButton::clicked, [&] {
        ot_.addContact(
            ui_->contactLabelText->text(), ui_->paymentCodeText->text());
        close();
    });
    connect(cancel, &QPushButton::clicked, [&] { close(); });
}

AddContact::~AddContact() = default;
}  // namespace metier::widget
