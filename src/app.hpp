// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QObject>
#include <QString>
#include <memory>

namespace metier
{
class App final : public QObject
{
    Q_OBJECT

signals:
    void startup();

public slots:
    void displayFirstRun();
    void displayNamePrompt();
    void displayBlockchainChooser();
    void displayMainWindow();

public:
    struct Imp;

    static App* Get(int argc = 0, char* argv[] = nullptr) noexcept;
    static void Cleanup() noexcept;

    auto run() -> int;

    ~App() final;

private:
    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    App(int& argc, char** argv) noexcept;
};
}  // namespace metier
