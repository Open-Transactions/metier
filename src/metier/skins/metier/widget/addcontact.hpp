// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QDialog>
#include <QObject>
#include <QString>
#include <memory>

namespace metier
{
namespace common
{
class Api;
}  // namespace common
}  // namespace metier

namespace Ui
{
class AddContact;
}

namespace metier::skins::metier::widget
{
class AddContact final : public QDialog
{
    Q_OBJECT

public Q_SLOTS:
    void addContact(QString label, QString data);

private Q_SLOTS:
    void addContactPrivate();

public:
    AddContact(QObject* parent, common::Api& ot);

    ~AddContact() final;

private:
    std::unique_ptr<Ui::AddContact> ui_;
    common::Api& ot_;

    AddContact(const AddContact&) = delete;
    AddContact(AddContact&&) = delete;
    AddContact& operator=(const AddContact&) = delete;
    AddContact& operator=(AddContact&&) = delete;
};
}  // namespace metier::skins::metier::widget
