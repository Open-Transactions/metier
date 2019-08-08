// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QAbstractItemModel>
#include <QObject>
#include <QString>
#include <QVariant>

namespace metier::model
{
class Licenses final : public QAbstractListModel
{
    Q_OBJECT
public:
    auto data(const QModelIndex& index, int role) const -> QVariant final;
    auto rowCount(const QModelIndex& parent) const -> int final;
    auto suggestedWidthInCharacters() const noexcept -> int;

    Licenses() noexcept = default;

private:
    using ot_super = QAbstractListModel;

    Licenses(const Licenses&) = delete;
    Licenses(Licenses&&) = delete;
    Licenses& operator=(const Licenses&) = delete;
    Licenses& operator=(Licenses&&) = delete;
};
}  // namespace metier::model
