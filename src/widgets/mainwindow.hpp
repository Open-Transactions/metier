// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QMainWindow>
#include <QObject>
#include <QString>
#include <memory>

class QItemSelection;
class QWidget;

namespace metier
{
class OTWrap;
}

namespace metier::widget
{
class MainWindow final : public QMainWindow
{
    Q_OBJECT

public slots:

private slots:
    void accountListUpdated(
        const QItemSelection& current,
        const QItemSelection& previous);
    void initModels();
    void showBlockchainChooser();
    void showLicenseViewer();
    void showRecoveryWords();
    void changeChain();
    void updateToolbox();

public:
    MainWindow(QWidget* parent, OTWrap& ot) noexcept;

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
}  // namespace metier::widget
