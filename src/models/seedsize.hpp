// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QAbstractItemModel>
#include <QObject>
#include <QString>
#include <QVariant>
#include <string>
#include <utility>
#include <vector>

namespace metier::model
{
class SeedSize final : public QAbstractListModel
{
    Q_OBJECT
public:
    using Data = std::vector<std::pair<std::string, int>>;

    auto data(const QModelIndex& index, int role) const -> QVariant final;
    auto rowCount(const QModelIndex& parent) const -> int final;
    auto suggestedWidthInCharacters() const noexcept -> int { return max_; }

    SeedSize(QObject* parent, Data&& data) noexcept;

private:
    using ot_super = QAbstractListModel;

    const Data data_;
    const int max_;

    SeedSize(const SeedSize&) = delete;
    SeedSize(SeedSize&&) = delete;
    SeedSize& operator=(const SeedSize&) = delete;
    SeedSize& operator=(SeedSize&&) = delete;
};
}  // namespace metier::model
