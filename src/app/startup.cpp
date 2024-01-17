// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "app/startup.hpp"  // IWYU pragma: associated

#include <QDebug>

namespace metier
{
auto Startup::doDisplayBlockchainChooser() -> void
{
    qDebug() << "Q_EMITting startup.displayBlockchainChooser()";
    Q_EMIT displayBlockchainChooser();
}

auto Startup::doDisplayFirstRun() -> void
{
    qDebug() << "Q_EMITting startup.displayFirstRun()";
    Q_EMIT displayFirstRun();
}

auto Startup::doDisplayMainWindow() -> void
{
    qDebug() << "Q_EMITting startup.displayMainWindow()";
    Q_EMIT displayMainWindow();
}

auto Startup::doDisplayNamePrompt() -> void
{
    qDebug() << "Q_EMITting startup.displayNamePrompt()";
    Q_EMIT displayNamePrompt();
}

Startup::~Startup() = default;
}  // namespace metier
