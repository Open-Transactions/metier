// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opentxs/opentxs.hpp>

class MetierCallback : public opentxs::OTCallback
{
public:
    void runOne(
        const char* szDisplay,
        opentxs::Secret& theOutput,
        const std::string& key) const override;

    void runTwo(
        const char* szDisplay,
        opentxs::Secret& theOutput,
        const std::string& key) const override;

    MetierCallback();
};
