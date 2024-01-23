// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/common/model/licenses.hpp"  // IWYU pragma: associated

#include <QObject>
#include <QString>
#include <cstddef>
#include <string_view>
#include <utility>

#include "metier/common/license.hpp"

namespace metier::common::model
{
Licenses::Licenses(QObject* parent) noexcept { moveToThread(parent->thread()); }

auto Licenses::data(const QModelIndex& index, int role) const -> QVariant
{
    static const auto& data = MetierLicenseData();

    if (false == index.isValid()) { return {}; }

    const auto row = static_cast<std::size_t>(index.row());

    switch (role) {
        case Qt::DisplayRole: {
            try {
                const auto& value = data.at(row).first;

                return QString::fromLocal8Bit(
                    value.data(), static_cast<int>(value.size()));
            } catch (...) {
            }
        } break;
        case Qt::UserRole: {
            try {
                const auto& value = data.at(row).second;

                return QString::fromLocal8Bit(
                    value.data(), static_cast<int>(value.size()));
            } catch (...) {
            }
        } break;
        default: {
        }
    }

    return {};
}

auto Licenses::rowCount([[maybe_unused]] const QModelIndex& parent) const -> int
{
    static const auto output = MetierLicenseData().size();

    return static_cast<int>(output);
}

auto Licenses::suggestedWidthInCharacters() const noexcept -> int
{
    return static_cast<int>(LongestLicenseDescription(MetierLicenseData()));
}
}  // namespace metier::common::model
