// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <memory>

namespace metier::common
{
class App final : public QObject
{
    Q_OBJECT

Q_SIGNALS:
    void passwordPrompt(QString prompt, bool once);
    void startup();

public Q_SLOTS:
    void displayBlockchainChooser();
    void displayFirstRun();
    void displayMainWindow();
    void displayNamePrompt();
    void needPasswordPrompt(QString prompt, bool once);

private Q_SLOTS:
    void displayPasswordPrompt(QString prompt, bool once);

public:
    struct Imp;

    static App* Get(int& argc, char** argv) noexcept;
    static void Cleanup() noexcept;

    auto confirmPassword(QString prompt, QString key) -> QString;
    auto getPassword(QString prompt, QString key) -> QString;
    auto run() -> int;

    ~App() final;

private:
    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    App(int& argc, char** argv) noexcept;
};
}  // namespace metier::common
