// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QIdentityProxyModel>
#include <QString>
#include <memory>

namespace opentxs
{
namespace ui
{
class ProfileQt;
}  // namespace ui
}  // namespace opentxs

namespace metier::model
{
class Profile final : public QIdentityProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ displayName NOTIFY updated)
    Q_PROPERTY(QString paymentCode READ paymentCode NOTIFY updated)

signals:
    void updated() const;

public:
    QString displayName() const noexcept;
    QString paymentCode() const noexcept;

    Profile(opentxs::ui::ProfileQt* source) noexcept;

    ~Profile() final;

private:
    using ot_super = QIdentityProxyModel;

    struct Imp;

    std::unique_ptr<Imp> imp_;

    Profile(const Profile&) = delete;
    Profile(Profile&&) = delete;
    Profile& operator=(const Profile&) = delete;
    Profile& operator=(Profile&&) = delete;
};
}  // namespace metier::model
