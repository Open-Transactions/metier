// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/common/passwordcallback.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QString>

#include "metier/common/app.hpp"

namespace metier::common
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
    opentxs::Secret& output,
    std::string_view prompt,
    std::string_view key) const noexcept -> void
{
    const auto data = imp_->app_.getPassword(
        QString::fromStdString(std::string{prompt}),
        QString::fromStdString(std::string{key}));
    output.AssignText(data.toStdString());
}

auto PasswordCallback::runTwo(
    opentxs::Secret& output,
    std::string_view prompt,
    std::string_view key) const noexcept -> void
{
    const auto data = imp_->app_.confirmPassword(
        QString::fromStdString(std::string{prompt}),
        QString::fromStdString(std::string{key}));
    output.AssignText(data.toStdString());
}

PasswordCallback::~PasswordCallback() = default;
}  // namespace metier::common
