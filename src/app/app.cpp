// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "app.hpp"  // IWYU pragma: associated

#include <otwrap.hpp>
#include <mutex>

#include "app/imp.hpp"

namespace metier
{
std::unique_ptr<App> App::Imp::singleton_{};

App::App(int& argc, char** argv) noexcept
    : imp_p_(Imp::factory(*this, argc, argv))
    , imp_(*imp_p_)
{
    imp_.init(argc, argv);
    auto* ot = imp_.otwrap();
    connect(ot, &OTWrap::needSeed, this, &App::displayFirstRun);
    connect(ot, &OTWrap::needProfileName, this, &App::displayNamePrompt);
    connect(ot, &OTWrap::needBlockchain, this, &App::displayBlockchainChooser);
    connect(ot, &OTWrap::readyForMainWindow, this, &App::displayMainWindow);
    connect(this, &App::startup, ot, &OTWrap::checkStartupConditions);
    emit startup();
}

auto App::Cleanup() noexcept -> void { Imp::singleton_.reset(); }

auto App::displayBlockchainChooser() -> void
{
    imp_.displayBlockchainChooser();
}

auto App::displayFirstRun() -> void { imp_.displayFirstRun(); }

auto App::displayMainWindow() -> void { imp_.displayMainWindow(); }

auto App::displayNamePrompt() -> void { imp_.displayNamePrompt(); }

auto App::confirmPassword(QString prompt, QString key) -> QString
{
    return imp_.confirmPassword(prompt, key);
}

auto App::getPassword(QString prompt, QString key) -> QString
{
    return imp_.getPassword(prompt, key);
}

auto App::run() -> int { return imp_.run(); }

auto App::Get(int argc, char* argv[]) noexcept -> App*
{
    static std::mutex mutex{};
    std::lock_guard<std::mutex> lock(mutex);
    auto& singleton = Imp::singleton_;

    if (!singleton) { singleton.reset(new App{argc, argv}); }

    return singleton.get();
}

App::~App()
{
    imp_p_.reset();
    OTWrap::Cleanup();
}
}  // namespace metier
