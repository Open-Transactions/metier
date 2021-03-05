// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QIdentityProxyModel>
#include <QObject>

class QModelIndex;
class QAbstractItemModel;

namespace metier::model
{
class BlockchainChooser final : public QIdentityProxyModel
{
    Q_OBJECT
public:
    auto flags(const QModelIndex&) const -> Qt::ItemFlags final { return {}; }

    BlockchainChooser(QAbstractItemModel* parent) noexcept
    {
        setSourceModel(parent);
    }

    ~BlockchainChooser() final = default;

private:
    BlockchainChooser(const BlockchainChooser&) = delete;
    BlockchainChooser(BlockchainChooser&&) = delete;
    BlockchainChooser& operator=(const BlockchainChooser&) = delete;
    BlockchainChooser& operator=(BlockchainChooser&&) = delete;
};
}  // namespace metier::model
