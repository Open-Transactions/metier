// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "otwrap.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QDebug>
#include <QGuiApplication>
#include <utility>

#include "models/blockchainchooser.hpp"
#include "otwrap/imp.hpp"
#include "util/convertblockchain.hpp"

namespace metier
{
OTWrap::OTWrap(QGuiApplication& parent)
    : QObject(&parent)
    , imp_p_(std::make_unique<Imp>(parent, *this))
    , imp_(*imp_p_)
{
}

auto OTWrap::accountActivityModel(const QString& id) -> model::AccountActivity*
{
    return imp_.accountActivityModel(
        imp_.api_.Factory().Identifier(id.toStdString()));
}

auto OTWrap::accountActivityModel(const int chain) -> model::AccountActivity*
{
    return imp_.accountActivityModel(
        imp_.api_.UI().BlockchainAccountID(util::convert(chain)));
}

auto OTWrap::accountListModel() -> QAbstractItemModel*
{
    // TODO return a proxy model instead maybe

    return imp_.api_.UI().AccountListQt(imp_.nym_id_);
}

auto OTWrap::blockchainChooserModel(const bool testnet) -> QAbstractItemModel*
{
    return (testnet ? imp_.blockchain_chooser_testnet_
                    : imp_.blockchain_chooser_mainnet_)
        .get();
}

auto OTWrap::checkChainCount() -> void
{
    emit chainsChanged(imp_.scanBlockchains().first);
}

auto OTWrap::checkStartupConditions() -> void
{
    if (imp_.needSeed()) {
        emit needSeed();

        return;
    }

    if (false == imp_.validateSeed()) {
        qFatal("Unable to initialize wallet seed");
    }

    if (imp_.needNym()) {
        emit needProfileName();

        return;
    }

    if (false == imp_.validateNym()) {
        qFatal("Unable to initialize identity");
    }

    emit nymReady();

    if (0 == imp_.scanBlockchains().first) {
        emit needBlockchain();

        return;
    }

    if (false == imp_.validateBlockchains()) {
        qFatal("Unable to initialize blockchains");
    }

    emit readyForMainWindow();
}

auto OTWrap::checkAccounts() -> void { imp_.validateBlockchains(); }

auto OTWrap::Cleanup() noexcept -> void { ot::Cleanup(); }

auto OTWrap::convertBlockchainAccountID(const QString& id) -> int
{
    const auto accountID = imp_.api_.Factory().Identifier(id.toStdString());

    return static_cast<int>(imp_.api_.UI().BlockchainAccountToChain(accountID));
}

auto OTWrap::createNewSeed(const int type, const int lang, const int strength)
    -> QStringList
{
    return imp_.createNewSeed(type, lang, strength);
}

auto OTWrap::createNym(QString alias) -> void
{
    imp_.createNym(alias);
    checkStartupConditions();
}

auto OTWrap::enabledBlockchains() -> EnabledBlockchains
{
    return imp_.enabled_chains_.get();
}

auto OTWrap::enabledCurrencyCount() -> int
{
    return imp_.scanBlockchains().first;
}

auto OTWrap::importSeed(int type, int lang, QString words) -> void
{
    imp_.importSeed(type, lang, words);
    checkStartupConditions();
}

auto OTWrap::longestBlockchainName() -> int
{
    return imp_.scanBlockchains().second;
}

auto OTWrap::longestSeedWord() -> int { return imp_.longest_seed_word_; }

auto OTWrap::seedLanguageModel(const int type) -> model::SeedLanguage*
{
    return imp_.seedLanguageModel(type);
}

auto OTWrap::seedSizeModel(const int type) -> model::SeedSize*
{
    return imp_.seedSizeModel(type);
}

auto OTWrap::seedTypeModel() -> model::SeedType*
{
    return imp_.seed_type_.get();
}

auto OTWrap::seedWordValidator(const int type, const int lang) -> QValidator*
{
    return imp_.seedWordValidator(type, lang);
}

auto OTWrap::wordCount(const int type, const int strength) -> int
{
    return imp_.wordCount(type, strength);
}

OTWrap::~OTWrap() = default;
}  // namespace metier
