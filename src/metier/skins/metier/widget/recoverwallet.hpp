// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QDialog>
#include <QMetaObject>
#include <memory>

class QObject;

namespace metier
{
namespace common
{
class Api;
}  // namespace common
}  // namespace metier

namespace metier::skins::metier::widget
{
class RecoverWallet final : public QDialog
{
    Q_OBJECT

public Q_SLOTS:
    void cancel();

public:
    RecoverWallet(QObject* parent, common::Api& ot) noexcept;

    ~RecoverWallet() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    RecoverWallet(const RecoverWallet&) = delete;
    RecoverWallet(RecoverWallet&&) = delete;
    RecoverWallet& operator=(const RecoverWallet&) = delete;
    RecoverWallet& operator=(RecoverWallet&&) = delete;
};
}  // namespace metier::skins::metier::widget
