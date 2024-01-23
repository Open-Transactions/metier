// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QDialog>
#include <QObject>
#include <QString>
#include <memory>

class QWidget;

namespace Ui
{
class ProfileAlias;
}

namespace metier::widget
{
class ProfileAlias final : public QDialog
{
    Q_OBJECT

Q_SIGNALS:
    void gotAlias(QString alias);

public:
    ProfileAlias(QObject* parent);

    ~ProfileAlias() final;

private:
    std::unique_ptr<Ui::ProfileAlias> ui_;

    auto check() noexcept -> void;
    auto confirm() noexcept -> void;

    ProfileAlias(const ProfileAlias&) = delete;
    ProfileAlias(ProfileAlias&&) = delete;
    ProfileAlias& operator=(const ProfileAlias&) = delete;
    ProfileAlias& operator=(ProfileAlias&&) = delete;
};
}  // namespace metier::widget
