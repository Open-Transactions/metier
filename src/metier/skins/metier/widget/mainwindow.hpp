// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QMainWindow>
#include <QMetaObject>
#include <QString>
#include <memory>

class QCloseEvent;
class QItemSelection;
class QObject;

namespace metier
{
namespace common
{
class Api;
}  // namespace common
}  // namespace metier

namespace metier::skins::metier::widget
{
class MainWindow final : public QMainWindow
{
    Q_OBJECT

Q_SIGNALS:
    void progMaxUpdated(int);
    void progValueUpdated(int);

public Q_SLOTS:
    void setProgressMax(int);
    void setProgressValue(int);

private Q_SLOTS:
    void accountListUpdated(
        const QItemSelection& current,
        const QItemSelection& previous);
    void canMessage(bool value);
    void clearActivityThread();
    void contactListUpdated(
        const QItemSelection& current,
        const QItemSelection& previous);
    void exit();
    void initModels();
    void sendMessage();
    void showAddContact();
    void showAccountActivity(int chain);
    void showAccountActivity(QString account);
    void showActivityThread(QString contact);
    void showBlockchainChooser();
    void showBlockchainStatistics();
    void showLicenseViewer();
    void showRecoveryWords();
    void showSeedManager();
    void changeChain();
    void updateToolbox();
    void updateName(QString value);
    void updatePaymentCode(QString value);

public:
    auto closeEvent(QCloseEvent* event) -> void final;

    MainWindow(QObject* parent, common::Api& ot) noexcept;

    ~MainWindow() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
};
}  // namespace metier::skins::metier::widget
