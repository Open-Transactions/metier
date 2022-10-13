// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opentxs/opentxs.hpp>
#include <vector>

namespace metier
{
auto SeedEndpoints() noexcept -> const std::vector<const char*>&;
auto DefaultBlockchains() noexcept
    -> const std::vector<opentxs::blockchain::Type>&;
}  // namespace metier
