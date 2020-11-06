// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QApplication>
#include <QObject>
#include <QString>
#include <memory>

namespace metier
{
class App final : public QApplication
{
    Q_OBJECT

signals:
    void startup();

private slots:
    void displayBlockchainChooser();
    void displayFirstRun();
    void displayMainWindow();
    void displayNamePrompt();
    void displayNewSeed();
    void displayRecovery();

public:
    static App* Get(int argc = 0, char* argv[] = nullptr) noexcept;
    static void Cleanup() noexcept;

    ~App();

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    App(int& argc, char** argv) noexcept;
};
}  // namespace metier
