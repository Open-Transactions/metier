// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opentxs/opentxs.hpp>
#include <filesystem>
#include <iosfwd>
#include <string_view>
#include <utility>
#include <vector>

namespace metier::common
{
using LicenseData = std::vector<std::pair<std::string_view, std::string_view>>;

auto LongestLicenseDescription(const LicenseData& in) noexcept -> std::size_t;
auto MetierLicenseData() noexcept -> const LicenseData&;
auto license_from_cmrc(
    std::string_view dependency,
    const std::filesystem::path& filename,
    LicenseData& out) noexcept -> void;
}  // namespace metier::common
