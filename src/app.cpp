// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "app.hpp"  // IWYU pragma: associated

#include <otwrap.hpp>
#include <QPushButton>
#include <mutex>

#include "app_imp.hpp"
#include "util/focuser.hpp"
#include "widgets/blockchainchooser.hpp"
#include "widgets/firstrun.hpp"
#include "widgets/mainwindow.hpp"
#include "widgets/profilealias.hpp"

namespace metier
{
std::unique_ptr<App> App::Imp::singleton_{};

App::App(int& argc, char** argv) noexcept
    : QApplication(argc, argv)
    , imp_p_(std::make_unique<Imp>(*this))
    , imp_(*imp_p_)
{
    auto* ot = imp_.ot_.get();
    auto* first = imp_.first_run_.get();
    auto* alias = imp_.profile_alias_.get();
    auto* ok = imp_.blockchains_->Ok();
    connect(ot, &OTWrap::needSeed, this, &App::displayFirstRun);
    connect(ot, &OTWrap::needProfileName, this, &App::displayNamePrompt);
    connect(ot, &OTWrap::needBlockchain, this, &App::displayBlockchainChooser);
    connect(ot, &OTWrap::readyForMainWindow, this, &App::displayMainWindow);
    connect(first, &widget::FirstRun::wantNew, ot, &OTWrap::createSeed);
    connect(first, &widget::FirstRun::wantOld, this, &App::displayRecovery);
    connect(alias, &widget::ProfileAlias::gotAlias, ot, &OTWrap::createNym);
    connect(ok, &QPushButton::clicked, this, &App::startup);
    connect(this, &App::startup, ot, &OTWrap::checkStartupConditions);
    emit startup();
}

auto App::Cleanup() noexcept -> void { Imp::singleton_.reset(); }

auto App::displayBlockchainChooser() -> void
{
    util::Focuser(imp_.blockchains_.get()).show();
}

auto App::displayFirstRun() -> void
{
    util::Focuser(imp_.first_run_.get()).show();
}

auto App::displayMainWindow() -> void
{
    util::Focuser(imp_.main_window_.get()).show();
}

auto App::displayNamePrompt() -> void
{
    util::Focuser(imp_.profile_alias_.get()).show();
}

auto App::displayRecovery() -> void
{
    // TODO show a dialog that collects the 12 - 24 word recovery seed
}

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
