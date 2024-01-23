// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QMetaObject>
#include <QObject>

namespace metier::common
{
class Startup final : public QObject
{
    Q_OBJECT

Q_SIGNALS:
    void displayBlockchainChooser();
    void displayFirstRun();
    void displayMainWindow();
    void displayNamePrompt();

public:
    auto doDisplayBlockchainChooser() -> void;
    auto doDisplayFirstRun() -> void;
    auto doDisplayMainWindow() -> void;
    auto doDisplayNamePrompt() -> void;

    ~Startup() final;
};
}  // namespace metier::common
