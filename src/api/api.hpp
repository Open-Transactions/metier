// Copyright (c) 2019-2022 The Open-Transactions developers
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
class IdentityManager;
class IdentityManagerQt;
class SeedTreeQt;
class SeedValidator;
}  // namespace ui
}  // namespace opentxs

namespace metier
{
namespace model
{
class SeedLanguage;
class SeedSize;
class SeedType;
}  // namespace model

class App;
}  // namespace metier

namespace metier
{
class Api final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int enabledCurrencyCount READ enabledCurrencyCount)
    Q_PROPERTY(int longestBlockchainName READ longestBlockchainName)
    Q_PROPERTY(int longestSeedWord READ longestSeedWord)
    Q_PROPERTY(QObject* identityManager READ identityManagerQML CONSTANT)
    Q_PROPERTY(QObject* seedManager READ seedManagerQML CONSTANT)
    Q_PROPERTY(QString appDomain READ Domain CONSTANT)
    Q_PROPERTY(QString appName READ Name CONSTANT)
    Q_PROPERTY(QString appTitle READ Title CONSTANT)

Q_SIGNALS:
    void chainsChanged(int enabledCount);

    void privateNeedBlockchain(QPrivateSignal);
    void privateNeedProfileName(QPrivateSignal);
    void privateNeedSeed(QPrivateSignal);
    void privateReadyForMainWindow(QPrivateSignal);

public Q_SLOTS:
    void checkAccounts();
    void checkStartupConditions();
    void createNym(QString alias);
    void importSeed(int type, int lang, QString words, QString password);
    void quit();
    void rescanBlockchain(int chain);
    void seedBackupFinished();

private Q_SLOTS:
    void chainIsDisabled(int chain);
    void chainIsEnabled(int chain);
    void checkChains(int chain);
    void doNeedNym();
    void doNeedSeed();

public:
    using BlockchainList = QVector<int>;

    static auto Cleanup() noexcept -> void;
    static auto Domain() noexcept -> QString;
    static auto Name() noexcept -> QString;
    static auto Title() noexcept -> QString;

    auto blockchainChooserModel(const bool testnet) -> QAbstractItemModel*;
    auto blockchainStatisticsModel() -> QAbstractItemModel*;
    auto identityManager() noexcept -> opentxs::ui::IdentityManagerQt*;
    auto seedLanguageModel(const int type) -> model::SeedLanguage*;
    auto seedManager() noexcept -> opentxs::ui::SeedTreeQt*;
    auto seedSizeModel(const int type) -> model::SeedSize*;
    auto seedTypeModel() -> model::SeedType*;
    auto seedWordValidator(const int type, const int lang)
        -> const opentxs::ui::SeedValidator*;

    Q_INVOKABLE QObject* blockchainChooserModelQML(bool testnet);
    Q_INVOKABLE QObject* blockchainStatisticsModelQML();
    Q_INVOKABLE QObject* identityManagerQML();
    Q_INVOKABLE QObject* seedLanguageModelQML(const int type);
    Q_INVOKABLE QObject* seedManagerQML();
    Q_INVOKABLE QObject* seedSizeModelQML(const int type);
    Q_INVOKABLE QObject* seedTypeModelQML();
    Q_INVOKABLE QObject* seedWordValidatorQML(const int type, const int lang);

    Q_INVOKABLE int accountIDtoBlockchainType(const QString& id);
    Q_INVOKABLE QString addContact(QString label, QString id);
    Q_INVOKABLE QString blockchainTypeToAccountID(int type);
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
    Q_INVOKABLE QString versionString(int suffix = 0);

    explicit Api(QGuiApplication& parent, App& app, int& argc, char** argv);

    ~Api() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    Api() = delete;
};
}  // namespace metier
