// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opentxs/opentxs.hpp>
#include <memory>

namespace metier
{
class App;
}  // namespace metier

namespace metier
{
class PasswordCallback final : public opentxs::PasswordCallback
{
public:
    void runOne(
        const char* szDisplay,
        opentxs::Secret& theOutput,
        const std::string& key) const final;
    void runTwo(
        const char* szDisplay,
        opentxs::Secret& theOutput,
        const std::string& key) const final;

    PasswordCallback(App& app);

    ~PasswordCallback() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_;

    PasswordCallback() = delete;
    PasswordCallback(const PasswordCallback&) = delete;
    PasswordCallback(PasswordCallback&&) = delete;
    auto operator=(const PasswordCallback&) -> PasswordCallback& = delete;
    auto operator=(PasswordCallback&&) -> PasswordCallback& = delete;
};
}  // namespace metier
