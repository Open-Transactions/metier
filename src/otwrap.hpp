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

namespace opentxs
{
namespace ui
{
class AccountActivityQt;
class SeedValidator;
}  // namespace ui
}  // namespace opentxs

namespace metier
{
namespace model
{
class AccountList;
class Profile;
class SeedLanguage;
class SeedSize;
class SeedType;
}  // namespace model

class App;
}  // namespace metier

namespace metier
{
class OTWrap final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int enabledCurrencyCount READ enabledCurrencyCount)
    Q_PROPERTY(int longestBlockchainName READ longestBlockchainName)
    Q_PROPERTY(int longestSeedWord READ longestSeedWord)

signals:
    void chainsChanged(int enabledCount);
    void needBlockchain();
    void needProfileName();
    void needSeed();
    void nymReady();
    void readyForMainWindow();

public slots:
    void checkAccounts();
    void checkStartupConditions();
    void createNym(QString alias);
    void importSeed(int type, int lang, QString words);

public:
    using BlockchainList = QVector<int>;
    using AccountActivity = opentxs::ui::AccountActivityQt;

    static auto Cleanup() noexcept -> void;

    AccountActivity* accountActivityModel(const QString& id);
    AccountActivity* accountActivityModel(const int chain);
    model::AccountList* accountListModel();
    QAbstractItemModel* blockchainChooserModel(const bool testnet);
    QAbstractItemModel* blockchainStatisticsModel();
    model::Profile* profileModel();
    model::SeedLanguage* seedLanguageModel(const int type);
    model::SeedSize* seedSizeModel(const int type);
    model::SeedType* seedTypeModel();
    const opentxs::ui::SeedValidator* seedWordValidator(
        const int type,
        const int lang);

    Q_INVOKABLE QObject* accountActivityModelQML(const QString& id);
    Q_INVOKABLE QObject* accountListModelQML();
    Q_INVOKABLE QObject* blockchainChooserModelQML(bool testnet);
    Q_INVOKABLE QObject* blockchainStatisticsModelQML();
    Q_INVOKABLE QObject* profileModelQML();
    Q_INVOKABLE QObject* seedLanguageModelQML(const int type);
    Q_INVOKABLE QObject* seedSizeModelQML(const int type);
    Q_INVOKABLE QObject* seedTypeModelQML();
    Q_INVOKABLE QObject* seedWordValidatorQML(const int type, const int lang);

    Q_INVOKABLE int convertBlockchainAccountID(const QString& id);
    Q_INVOKABLE QStringList
    createNewSeed(const int type, const int lang, const int strength);
    Q_INVOKABLE QStringList getRecoveryWords();
    Q_INVOKABLE int wordCount(const int type, const int strength);
    Q_INVOKABLE int enabledCurrencyCount();
    BlockchainList enabledBlockchains();
    Q_INVOKABLE QString getQRcodeBase64(const QString input_string);
    Q_INVOKABLE int longestBlockchainName();
    Q_INVOKABLE int longestSeedWord();
    Q_INVOKABLE void openSystemBrowserLink(QString url_link);
    BlockchainList validBlockchains();

    explicit OTWrap(QGuiApplication& parent, App& app, int& argc, char** argv);

    ~OTWrap() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    OTWrap() = delete;
};
}  // namespace metier
