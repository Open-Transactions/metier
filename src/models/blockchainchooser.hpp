// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QObject>
#include <QSortFilterProxyModel>
#include <QString>
#include <QVariant>

class QModelIndex;

namespace opentxs::api::client
{
class UI;
}

namespace metier::model
{
class BlockchainChooser final : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    auto columnCount([[maybe_unused]] const QModelIndex& parent) const
        -> int final
    {
        return 2;
    }
    auto data(const QModelIndex& index, int role = Qt::DisplayRole) const
        -> QVariant final;
    auto flags(const QModelIndex& index) const -> Qt::ItemFlags final;
    auto headerData(
        int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole) const -> QVariant final;
    auto setData(const QModelIndex& index, const QVariant& value, int role)
        -> bool final;

    BlockchainChooser(
        QObject& parent,
        const opentxs::api::client::UI& ui,
        const bool testnetMode);
    ~BlockchainChooser() final = default;

private:
    using ot_super = QSortFilterProxyModel;

    const bool testnetMode_;

    auto filterAcceptsColumn(
        int source_column,
        const QModelIndex& source_parent) const -> bool final;

    BlockchainChooser(const BlockchainChooser&) = delete;
    BlockchainChooser(BlockchainChooser&&) = delete;
    BlockchainChooser& operator=(const BlockchainChooser&) = delete;
    BlockchainChooser& operator=(BlockchainChooser&&) = delete;
};
}  // namespace metier::model
