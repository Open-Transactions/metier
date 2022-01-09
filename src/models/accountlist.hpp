// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QIdentityProxyModel>

namespace metier::model
{
class AccountList final : public QIdentityProxyModel
{
    Q_OBJECT

public:
    AccountList(QAbstractItemModel* source) noexcept;

    ~AccountList() override = default;

private:
    using ot_super = QIdentityProxyModel;

    AccountList(const AccountList&) = delete;
    AccountList(AccountList&&) = delete;
    AccountList& operator=(const AccountList&) = delete;
    AccountList& operator=(AccountList&&) = delete;
};
}  // namespace metier::model
