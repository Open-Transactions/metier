// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QObject>

namespace metier
{
class QmlInterface final : public QObject
{
    Q_OBJECT

signals:
    void displayBlockchainChooser();
    void displayFirstRun();
    void displayMainWindow();
    void displayNamePrompt();

public:
    auto doDisplayBlockchainChooser() -> void;
    auto doDisplayFirstRun() -> void;
    auto doDisplayMainWindow() -> void;
    auto doDisplayNamePrompt() -> void;

    ~QmlInterface() final;
};
}  // namespace metier
