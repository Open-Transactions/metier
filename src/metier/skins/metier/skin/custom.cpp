// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/common/custom.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>

namespace metier::common
{
auto SeedEndpoints() noexcept -> const std::vector<const char*>&
{
    static const auto data = std::vector<const char*>{};

    return data;
}

auto DefaultBlockchains() noexcept
    -> const std::vector<opentxs::blockchain::Type>&
{
    static const auto data = std::vector<opentxs::blockchain::Type>{};

    return data;
}
}  // namespace metier::common
