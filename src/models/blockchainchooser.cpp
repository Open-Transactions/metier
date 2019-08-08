// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "blockchainchooser.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QDebug>

using OTModel = opentxs::ui::BlockchainSelectionQt;

namespace metier::model
{
BlockchainChooser::BlockchainChooser(QObject& parent, const bool testnetMode)
    : ot_super(&parent)
    , testnetMode_(testnetMode)
{
}

auto BlockchainChooser::data(const QModelIndex& index, int role) const
    -> QVariant
{
    if (false == index.isValid()) { return {}; }

    auto sourceIndex = mapToSource(index);
    auto model = sourceModel();

    if (1 == index.column()) {
        switch (role) {
            case Qt::CheckStateRole: {
                const auto revisedIndex = model->index(
                    sourceIndex.row(),
                    OTModel::EnabledColumn,
                    sourceIndex.parent());
                const auto enabled =
                    model->data(revisedIndex, Qt::DisplayRole).toBool();

                return enabled ? Qt::Checked : Qt::Unchecked;
            }
            case Qt::TextAlignmentRole: {

                return Qt::AlignCenter;
            }
            default: {

                return {};
            }
        }
    }

    return ot_super::data(index, role);
}

auto BlockchainChooser::filterAcceptsColumn(
    int source_column,
    [[maybe_unused]] const QModelIndex& source_parent) const -> bool
{
    return (
        source_column == OTModel::NameColumn ||
        source_column == OTModel::EnabledColumn);
}

auto BlockchainChooser::filterAcceptsRow(
    int sourceRow,
    const QModelIndex& sourceParent) const -> bool
{
    const auto* pSourceModel = sourceModel();

    OT_ASSERT(nullptr != pSourceModel)

    const auto indexTestnet =
        pSourceModel->index(sourceRow, OTModel::TestnetColumn, sourceParent);
    const auto qvarTestnet = pSourceModel->data(indexTestnet, Qt::DisplayRole);
    const auto testnetMode =
        qvarTestnet.isValid() ? qvarTestnet.toBool() : false;

    return (testnetMode == testnetMode_);
}

auto BlockchainChooser::flags(const QModelIndex& index) const -> Qt::ItemFlags
{
    // TODO enable main net currencies at a later date
    if (false == testnetMode_) { return {}; }

    if (OTModel::EnabledColumn == index.column()) {

        return ot_super::flags(index) | Qt::ItemIsSelectable |
               Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    }

    return ot_super::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

auto BlockchainChooser::headerData(
    int section,
    Qt::Orientation orientation,
    int role) const -> QVariant
{
    auto varHeaderData = ot_super::headerData(section, orientation, role);

    if (Qt::Horizontal == orientation && Qt::DisplayRole == role) {
        if (OTModel::NameColumn == section) {

            return QVariant(tr("Blockchain"));
        } else if (OTModel::EnabledColumn == section)

            return QVariant(tr("Enabled"));
    }

    return varHeaderData;
}

auto BlockchainChooser::setData(
    const QModelIndex& index,
    const QVariant& value,
    int role) -> bool
{
    if (!index.isValid()) { return false; }

    auto sourceIndex = mapToSource(index);
    auto pSourceModel = sourceModel();

    if (role == Qt::CheckStateRole) {
        if (OTModel::EnabledColumn == sourceIndex.column()) {
            const auto chain =
                pSourceModel->data(sourceIndex, OTModel::TypeRole).toInt();
            auto ret = bool{};

            if (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked) {
                // user has checked item
                QMetaObject::invokeMethod(
                    pSourceModel,
                    "enableChain",
                    Q_RETURN_ARG(bool, ret),
                    Q_ARG(int, chain));
            } else {
                // user has unchecked item
                QMetaObject::invokeMethod(
                    pSourceModel,
                    "disableChain",
                    Q_RETURN_ARG(bool, ret),
                    Q_ARG(int, chain));
            }

            return true;
        }
    }

    return false;
}
}  // namespace metier::model
