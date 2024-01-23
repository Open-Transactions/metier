// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opentxs/opentxs.hpp>
#include <cstdint>

namespace ot = opentxs;

namespace metier::common
{
constexpr auto convert(const int chain)
{
    return static_cast<ot::blockchain::Type>(static_cast<std::uint32_t>(chain));
}

constexpr auto convert(const ot::blockchain::Type chain)
{
    return static_cast<int>(static_cast<std::uint32_t>(chain));
}
}  // namespace metier::common
