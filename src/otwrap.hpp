// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVector>
#include <memory>

class QAbstractItemModel;
class QGuiApplication;
class QValidator;

namespace metier
{
namespace model
{
class AccountActivity;
class AccountList;
class BlockchainChooser;
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
    Q_PROPERTY(int enabledCurrencyCount READ enabledCurrencyCount)
    Q_PROPERTY(QVector<int> enabledBlockchains READ enabledBlockchains)
    Q_PROPERTY(int longestBlockchainName READ longestBlockchainName)
    Q_PROPERTY(int longestSeedWord READ longestSeedWord)
    Q_PROPERTY(QVector<int> validBlockchains READ validBlockchains)

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
    using BlockchainList = QVector<int>;

    static auto Cleanup() noexcept -> void;

    model::AccountActivity* accountActivityModel(const QString& id);
    model::AccountActivity* accountActivityModel(const int chain);
    model::AccountList* accountListModel();
    model::BlockchainChooser* blockchainChooserModel(const bool testnet);
    Q_INVOKABLE int convertBlockchainAccountID(const QString& id);
    Q_INVOKABLE QStringList
    createNewSeed(const int type, const int lang, const int strength);
    int enabledCurrencyCount();
    BlockchainList enabledBlockchains();
    int longestBlockchainName();
    int longestSeedWord();
    model::SeedLanguage* seedLanguageModel(const int type);
    model::SeedSize* seedSizeModel(const int type);
    model::SeedType* seedTypeModel();
    QValidator* seedWordValidator(const int type, const int lang);
    BlockchainList validBlockchains();
    Q_INVOKABLE int wordCount(const int type, const int strength);

    explicit OTWrap(QGuiApplication& parent);

    ~OTWrap() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    OTWrap() = delete;
};
}  // namespace metier
