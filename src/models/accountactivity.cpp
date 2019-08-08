// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "accountactivity.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>

using OTModel = opentxs::ui::AccountActivityQt;

namespace metier::model
{
AccountActivity::AccountActivity()
    : ot_super(nullptr)
    , ot_model_(nullptr)
{
}

QVariant AccountActivity::data(const QModelIndex& index, int role) const
{
    if (false == index.isValid()) { return {}; }

    auto sourceIndex = mapToSource(index);
    auto model = sourceModel();

    if (Qt::DisplayRole != role) { return {}; }

    switch (index.column()) {
        case 0: {
            const auto revisedIndex = model->index(
                sourceIndex.row(), OTModel::TimeColumn, sourceIndex.parent());

            return model->data(revisedIndex, role);
        }
        case 1: {
            const auto revisedIndex = model->index(
                sourceIndex.row(), OTModel::AmountColumn, sourceIndex.parent());

            return model->data(revisedIndex, role);
        }
        case 2: {
            const auto revisedIndex = model->index(
                sourceIndex.row(), OTModel::TextColumn, sourceIndex.parent());

            return model->data(revisedIndex, role);
        }
        default: {

            return {};
        }
    }
}

auto AccountActivity::filterAcceptsColumn(
    int source_column,
    [[maybe_unused]] const QModelIndex& source_parent) const -> bool
{
    switch (source_column) {
        case OTModel::AmountColumn:
        case OTModel::TextColumn:
        case OTModel::TimeColumn: {

            return true;
        }
        default: {

            return false;
        }
    }
}

auto AccountActivity::getDepositAddress(const int chain) const noexcept
    -> QString
{
    if (nullptr != ot_model_) {

        return ot_model_->getDepositAddress(chain);
    } else {

        return {};
    }
}

auto AccountActivity::sendToAddress(
    const QString& address,
    const QString& amount,
    const QString& memo) const noexcept -> bool
{
    return ot_model_->sendToAddress(address, amount, memo);
}

auto AccountActivity::setSourceModel(
    opentxs::ui::AccountActivityQt* sourceModel) -> void
{
    using OTModel = opentxs::ui::AccountActivityQt;

    ot_model_ = sourceModel;
    ot_super::setSourceModel(ot_model_);

    if (nullptr != ot_model_) {
        connect(
            ot_model_,
            &OTModel::syncProgressUpdated,
            this,
            &AccountActivity::sync);
    }
}

auto AccountActivity::validateAddress(const QString& address) const noexcept
    -> bool
{
    return ot_model_->validateAddress(address);
}

auto AccountActivity::validateAmount(const QString& amount) const noexcept
    -> QString
{
    return ot_model_->validateAmount(amount);
}
}  // namespace metier::model
