// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <iosfwd>
#include <string_view>
#include <utility>
#include <vector>

namespace metier
{
using LicenseData = std::vector<std::pair<std::string_view, std::string_view>>;

auto LongestLicenseDescription() noexcept -> std::size_t;
auto MetierLicenseData() noexcept -> const LicenseData&;
}  // namespace metier
