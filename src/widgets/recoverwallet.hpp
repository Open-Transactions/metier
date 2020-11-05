// Copyright (c) 2019-2020 The Open-Transactions developers
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
class OTWrap;
}  // namespace metier

namespace metier::widget
{
class RecoverWallet : public QDialog
{
    Q_OBJECT
public:
    RecoverWallet(OTWrap& ot) noexcept;

    ~RecoverWallet();

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    RecoverWallet(const RecoverWallet&) = delete;
    RecoverWallet(RecoverWallet&&) = delete;
    RecoverWallet& operator=(const RecoverWallet&) = delete;
    RecoverWallet& operator=(RecoverWallet&&) = delete;
};
}  // namespace metier::widget
