// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "licenses.hpp"  // IWYU pragma: associated

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "license.hpp"

namespace metier::model
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
                return QString::fromLocal8Bit(data.at(row).first.c_str());
            } catch (...) {
            }
        } break;
        case Qt::UserRole: {
            try {
                return QString{data.at(row).second.c_str()};
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
    return static_cast<int>(LongestLicenseDescription());
}
}  // namespace metier::model
