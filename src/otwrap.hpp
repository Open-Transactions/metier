// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <memory>

class QAbstractItemModel;
class QApplication;
class QValidator;

namespace metier
{
namespace model
{
class AccountActivity;
class SeedLanguage;
class SeedSize;
class SeedType;
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
    void importSeed(int type, int lang, QString words);

public:
    using EnabledBlockchains = QVector<int>;

    static auto Cleanup() noexcept -> void;

    Q_INVOKABLE model::AccountActivity* accountActivityModel(const QString& id);
    Q_INVOKABLE model::AccountActivity* accountActivityModel(const int chain);
    Q_INVOKABLE QAbstractItemModel* accountListModel();
    Q_INVOKABLE QAbstractItemModel* blockchainChooserModel(const bool testnet);
    Q_INVOKABLE int convertBlockchainAccountID(const QString& id);
    Q_INVOKABLE QStringList
    createNewSeed(const int type, const int lang, const int strength);
    Q_INVOKABLE int enabledCurrencyCount();
    Q_INVOKABLE EnabledBlockchains enabledBlockchains();
    Q_INVOKABLE int longestBlockchainName();
    Q_INVOKABLE int longestSeedWord();
    Q_INVOKABLE model::SeedLanguage* seedLanguageModel(const int type);
    Q_INVOKABLE model::SeedSize* seedSizeModel(const int type);
    Q_INVOKABLE model::SeedType* seedTypeModel();
    Q_INVOKABLE QValidator* seedWordValidator(const int type, const int lang);
    Q_INVOKABLE int wordCount(const int type, const int strength);

    explicit OTWrap(QApplication& parent);

    ~OTWrap() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    OTWrap() = delete;
};
}  // namespace metier
