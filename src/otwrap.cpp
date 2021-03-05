// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "otwrap.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QDebug>
#include <QGuiApplication>
#include <algorithm>
#include <utility>

#include "models/blockchainchooser.hpp"
#include "otwrap/imp.hpp"
#include "util/convertblockchain.hpp"

namespace metier
{
OTWrap::OTWrap(QGuiApplication& parent, int& argc, char** argv)
    : QObject(&parent)
    , imp_p_(std::make_unique<Imp>(parent, *this, argc, argv))
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

auto OTWrap::accountListModel() -> model::AccountList*
{
    OT_ASSERT(imp_.account_list_);

    return imp_.account_list_.get();
}

auto OTWrap::blockchainChooserModel(const bool testnet) -> QAbstractItemModel*
{
    if (testnet) {

        return imp_.api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::Test);
    } else {

        return imp_.mainnet_model_.get();
    }
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

    if (auto* model =
            imp_.api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::All);
        1 > model->enabledCount()) {
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

auto OTWrap::enabledBlockchains() -> BlockchainList
{
    return imp_.enabled_chains_.get();
}

auto OTWrap::enabledCurrencyCount() -> int
{
    return static_cast<int>(imp_.enabled_chains_.count());
}

auto OTWrap::getRecoveryWords() -> QStringList
{
    return imp_.getRecoveryWords();
}

auto OTWrap::importSeed(int type, int lang, QString words) -> void
{
    imp_.importSeed(type, lang, words);
    checkStartupConditions();
}

auto OTWrap::longestBlockchainName() -> int
{
    static const auto length = [&] {
        auto out = std::size_t{0};

        for (const auto chain : ot::blockchain::DefinedChains()) {
            out = std::max(out, ot::blockchain::DisplayString(chain).size());
        }

        return out;
    }();

    return static_cast<int>(length);
}

auto OTWrap::longestSeedWord() -> int { return imp_.longest_seed_word_; }

auto OTWrap::profileModel() -> model::Profile*
{
    OT_ASSERT(imp_.profile_);

    return imp_.profile_.get();
}

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

auto OTWrap::seedWordValidator(const int type, const int lang)
    -> const opentxs::ui::SeedValidator*
{
    return imp_.seedWordValidator(type, lang);
}

auto OTWrap::validBlockchains() -> BlockchainList
{
    auto output = BlockchainList{};
    const auto& input = ot::blockchain::SupportedChains();
    std::transform(
        input.begin(),
        input.end(),
        std::back_inserter(output),
        [](const auto chain) { return util::convert(chain); });

    return output;
}

auto OTWrap::wordCount(const int type, const int strength) -> int
{
    return imp_.wordCount(type, strength);
}

OTWrap::~OTWrap() = default;
}  // namespace metier
