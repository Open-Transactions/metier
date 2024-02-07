// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QStringList>
#include <memory>

class QAbstractItemModel;
class QGuiApplication;

namespace opentxs
{
namespace ui
{
class IdentityManagerQt;
class SeedTreeQt;
class SeedValidator;
}  // namespace ui
}  // namespace opentxs

namespace metier
{
namespace common
{
namespace model
{
class SeedLanguage;
class SeedSize;
class SeedType;
}  // namespace model

class App;
}  // namespace common
}  // namespace metier

namespace metier::common
{
class Api : public QObject
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
    virtual void checkAccounts();
    virtual void checkStartupConditions();
    virtual void createNym(QString alias);
    virtual void importSeed(
        int type,
        int lang,
        QString words,
        QString password);
    virtual void quit();
    virtual void rescanBlockchain(int chain);
    virtual void seedBackupFinished();

private Q_SLOTS:
    virtual void chainIsDisabled(int chain);
    virtual void chainIsEnabled(int chain);
    virtual void checkChains(int chain);
    virtual void doNeedNym();
    virtual void doNeedSeed();

public:
    struct Imp;

    using BlockchainList = QVector<int>;

    static auto Factory(
        QGuiApplication& parent,
        App& app,
        int& argc,
        char** argv) -> std::shared_ptr<Imp>;

    static auto Cleanup() noexcept -> void;
    static auto Domain() noexcept -> QString;
    static auto Name() noexcept -> QString;
    static auto Title() noexcept -> QString;

    virtual auto blockchainChooserModel(const bool testnet)
        -> QAbstractItemModel*;
    virtual auto blockchainStatisticsModel() -> QAbstractItemModel*;
    virtual auto identityManager() noexcept -> opentxs::ui::IdentityManagerQt*;
    virtual auto seedLanguageModel(const int type) -> model::SeedLanguage*;
    virtual auto seedManager() noexcept -> opentxs::ui::SeedTreeQt*;
    virtual auto seedSizeModel(const int type) -> model::SeedSize*;
    virtual auto seedTypeModel() -> model::SeedType*;
    virtual auto seedWordValidator(const int type, const int lang)
        -> const opentxs::ui::SeedValidator*;
    virtual auto enabledBlockchains() -> BlockchainList;
    virtual auto validBlockchains() -> BlockchainList;

    virtual Q_INVOKABLE QObject* blockchainChooserModelQML(bool testnet);
    virtual Q_INVOKABLE QObject* blockchainStatisticsModelQML();
    virtual Q_INVOKABLE QObject* identityManagerQML();
    virtual Q_INVOKABLE QObject* seedLanguageModelQML(const int type);
    virtual Q_INVOKABLE QObject* seedManagerQML();
    virtual Q_INVOKABLE QObject* seedSizeModelQML(const int type);
    virtual Q_INVOKABLE QObject* seedTypeModelQML();
    virtual Q_INVOKABLE QObject* seedWordValidatorQML(
        const int type,
        const int lang);

    virtual Q_INVOKABLE int accountIDtoBlockchainType(const QString& id);
    virtual Q_INVOKABLE QString addContact(QString label, QString id);
    virtual Q_INVOKABLE QString blockchainTypeToAccountID(int type);
    virtual Q_INVOKABLE QStringList
    createNewSeed(const int type, const int lang, const int strength);
    virtual Q_INVOKABLE QStringList getRecoveryWords();
    virtual Q_INVOKABLE int wordCount(const int type, const int strength);
    virtual Q_INVOKABLE int enabledCurrencyCount();
    virtual Q_INVOKABLE QString getQRcodeBase64(const QString input_string);
    virtual Q_INVOKABLE int longestBlockchainName();
    virtual Q_INVOKABLE int longestSeedWord();
    virtual Q_INVOKABLE void openSystemBrowserLink(QString url_link);
    virtual Q_INVOKABLE QString versionString(int suffix = 0);

    explicit Api(QGuiApplication& parent, std::shared_ptr<Imp> imp);
    Api() = delete;
    Api(const Api&) = delete;
    Api(Api&&) = delete;
    auto operator=(const Api&) -> Api& = delete;
    auto operator=(Api&&) -> Api& = delete;

    ~Api() override;

protected:
    std::shared_ptr<Imp> imp_p_;
    Imp& imp_;
};
}  // namespace metier::common
