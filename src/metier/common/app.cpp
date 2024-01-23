// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/common/app.hpp"  // IWYU pragma: associated

#include <QThread>
#include <future>
#include <mutex>

#include "metier/common/api.hpp"
#include "metier/common/app.imp.hpp"

namespace metier::common
{
std::unique_ptr<App> App::Imp::singleton_{};

App::App(int& argc, char** argv) noexcept
    : imp_p_(Imp::factory(*this, argc, argv))
    , imp_(*imp_p_)
{
    connect(this, &App::passwordPrompt, this, &App::displayPasswordPrompt);
    imp_.init(argc, argv);
    auto* ot = imp_.otwrap();
    connect(ot, &Api::privateNeedSeed, this, &App::displayFirstRun);
    connect(ot, &Api::privateNeedProfileName, this, &App::displayNamePrompt);
    connect(
        ot, &Api::privateNeedBlockchain, this, &App::displayBlockchainChooser);
    connect(ot, &Api::privateReadyForMainWindow, this, &App::displayMainWindow);
    connect(this, &App::startup, ot, &Api::checkStartupConditions);
    Q_EMIT startup();
}

auto App::Cleanup() noexcept -> void { Imp::singleton_.reset(); }

auto App::displayBlockchainChooser() -> void
{
    imp_.displayBlockchainChooser();
}

auto App::displayFirstRun() -> void { imp_.displayFirstRun(); }

auto App::displayMainWindow() -> void
{
    imp_.displayMainWindow();
    imp_.init_ = true;
    imp_.init_promise_.set_value();
}

auto App::displayNamePrompt() -> void { imp_.displayNamePrompt(); }

auto App::displayPasswordPrompt(QString prompt, bool once) -> void
{
    imp_.displayPasswordPrompt(prompt, once);
}

auto App::confirmPassword(QString prompt, QString key) -> QString
{
    return imp_.confirmPassword(prompt, key);
}

auto App::getPassword(QString prompt, QString key) -> QString
{
    return imp_.getPassword(prompt, key);
}

auto App::needPasswordPrompt(QString prompt, bool once) -> void
{
    if (QThread::currentThread() == this->thread()) {
        if (imp_.init_) {
            Q_EMIT passwordPrompt(prompt, once);
        } else {
            imp_.displayPasswordPrompt(prompt, once);
        }
    } else {
        imp_.init_future_.get();
        Q_EMIT passwordPrompt(prompt, once);
    }
}

auto App::run() -> int { return imp_.run(); }

auto App::Get(int& argc, char** argv) noexcept -> App*
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
    Api::Cleanup();
}
}  // namespace metier::common
