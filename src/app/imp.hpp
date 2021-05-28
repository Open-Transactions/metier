// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "app.hpp"  // IWYU pragma: associated

namespace metier
{
class OTWrap;
}

namespace metier
{
struct App::Imp {
    static std::unique_ptr<App> singleton_;

    bool init_{false};

    static auto factory(App& parent, int& argc, char** argv) noexcept
        -> std::unique_ptr<Imp>;

    virtual auto displayBlockchainChooser() -> void = 0;
    virtual auto displayFirstRun() -> void = 0;
    virtual auto displayMainWindow() -> void = 0;
    virtual auto displayNamePrompt() -> void = 0;
    virtual auto displayPasswordPrompt(QString prompt, bool once) -> void = 0;
    virtual auto confirmPassword(QString prompt, QString key) -> QString = 0;
    virtual auto getPassword(QString prompt, QString key) -> QString = 0;
    virtual auto init(int& argc, char** argv) noexcept -> void = 0;
    virtual auto run() -> int = 0;
    virtual auto otwrap() noexcept -> OTWrap* = 0;

    virtual ~Imp() = default;

protected:
    Imp() noexcept = default;
};
}  // namespace metier
