// Copyright (c) 2019-2022 The Open-Transactions developers
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
class SeedLanguage final : public QAbstractListModel
{
    Q_OBJECT
public:
    using Data = std::vector<std::pair<std::string, int>>;

    auto data(const QModelIndex& index, int role) const -> QVariant final;
    auto rowCount(const QModelIndex& parent) const -> int final;
    auto suggestedWidthInCharacters() const noexcept -> int { return max_; }

    SeedLanguage(QObject* parent, Data&& data) noexcept;
    ~SeedLanguage() final = default;

private:
    using ot_super = QAbstractListModel;

    const Data data_;
    const int max_;

    SeedLanguage(const SeedLanguage&) = delete;
    SeedLanguage(SeedLanguage&&) = delete;
    SeedLanguage& operator=(const SeedLanguage&) = delete;
    SeedLanguage& operator=(SeedLanguage&&) = delete;
};
}  // namespace metier::model
