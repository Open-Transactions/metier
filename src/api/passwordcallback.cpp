// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "api/passwordcallback.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QString>

#include "app/app.hpp"
#include "util/scopeguard.hpp"

namespace metier
{
struct PasswordCallback::Imp {
    App& app_;

    Imp(App& app) noexcept
        : app_(app)
    {
    }

private:
    Imp() = delete;
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;
};

PasswordCallback::PasswordCallback(App& app)
    : opentxs::PasswordCallback()
    , imp_(std::make_unique<Imp>(app))
{
    assert(imp_);
}

auto PasswordCallback::runOne(
    const char* prompt,
    opentxs::Secret& out,
    const std::string& key) const -> void
{
    const auto data = imp_->app_.getPassword(prompt, key.c_str());
    out.AssignText(data.toStdString());
}

auto PasswordCallback::runTwo(
    const char* prompt,
    opentxs::Secret& out,
    const std::string& key) const -> void
{
    const auto data = imp_->app_.confirmPassword(prompt, key.c_str());
    out.AssignText(data.toStdString());
}

PasswordCallback::~PasswordCallback() = default;
}  // namespace metier
