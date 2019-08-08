// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>

class QAbstractItemModel;
class QApplication;

namespace metier
{
namespace model
{
class AccountActivity;
}  // namespace model
}  // namespace metier

namespace metier
{
class OTWrap final : public QObject
{
    Q_OBJECT

signals:
    void chainsChanged(int enabledCount);
    void needBlockchain();
    void needProfileName();
    void needSeed();
    void nymReady();
    void readyForMainWindow();

public slots:
    void checkAccounts();
    void checkChainCount();
    void checkStartupConditions();
    void createNym(QString alias);
    void createSeed();

public:
    using EnabledBlockchains = QVector<int>;

    static auto Cleanup() noexcept -> void;

    Q_INVOKABLE model::AccountActivity* accountActivityModel(const QString& id);
    Q_INVOKABLE model::AccountActivity* accountActivityModel(const int chain);
    Q_INVOKABLE QAbstractItemModel* accountListModel();
    Q_INVOKABLE QAbstractItemModel* blockchainChooserModel(const bool testnet);
    Q_INVOKABLE int convertBlockchainAccountID(const QString& id);
    Q_INVOKABLE int enabledCurrencyCount();
    Q_INVOKABLE EnabledBlockchains enabledBlockchains();
    Q_INVOKABLE int longestBlockchainName();

    explicit OTWrap(QApplication& parent);

    ~OTWrap() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    OTWrap() = delete;
};
}  // namespace metier
