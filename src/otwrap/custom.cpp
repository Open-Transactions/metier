// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "otwrap/custom.hpp"  // IWYU pragma: associated

namespace metier
{
const char* introduction_notary_contract_{nullptr};

const char* messaging_notary_contract_{nullptr};

auto SeedEndpoints() noexcept -> const std::vector<char*>&
{
    static const auto data = std::vector<char*>{};

    return data;
}

auto DefaultBlockchains() noexcept
    -> const std::vector<opentxs::blockchain::Type>&
{
    static const auto data = std::vector<opentxs::blockchain::Type>{};

    return data;
}
}  // namespace metier
