// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QAbstractItemModel>
#include <QMetaObject>
#include <QVariant>
#include <string>
#include <utility>
#include <vector>

class QObject;

namespace metier::common::model
{
class SeedType final : public QAbstractListModel
{
    Q_OBJECT
public:
    using Data = std::vector<std::pair<std::string, int>>;

    auto data(const QModelIndex& index, int role) const -> QVariant final;
    auto rowCount(const QModelIndex& parent) const -> int final;
    auto suggestedWidthInCharacters() const noexcept -> int { return max_; }

    SeedType(QObject* parent, Data&& data) noexcept;

private:
    using ot_super = QAbstractListModel;

    const Data data_;
    const int max_;

    SeedType(const SeedType&) = delete;
    SeedType(SeedType&&) = delete;
    SeedType& operator=(const SeedType&) = delete;
    SeedType& operator=(SeedType&&) = delete;
};
}  // namespace metier::common::model
