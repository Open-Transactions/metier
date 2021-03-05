// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "otwrap.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QDebug>
#include <QDir>
#include <QGuiApplication>
#include <QStandardPaths>
#include <algorithm>
#include <map>
#include <mutex>
#include <set>
#include <string>

#include "deps/opentxs/tests/Cli.hpp"
#include "models/accountactivity.hpp"
#include "models/accountlist.hpp"
#include "models/profile.hpp"
#include "models/seedlang.hpp"
#include "models/seedsize.hpp"
#include "models/seedtype.hpp"
#include "util/claim.hpp"
#include "util/convertblockchain.hpp"
#include "util/scopeguard.hpp"

namespace ot = opentxs;

static const auto ot_args_ = ot::ArgList{};

auto make_args(QGuiApplication& parent, int& argc, char** argv) noexcept
    -> const ot::ArgList&;
auto make_args(QGuiApplication& parent, int& argc, char** argv) noexcept
    -> const ot::ArgList&
{
    parent.setOrganizationDomain(METIER_APP_DOMAIN);
    parent.setApplicationName(METIER_APP_NAME);
    auto path =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (path.isEmpty()) { qFatal("Invalid app data folder"); }

    auto folder = QDir{path.append("/opentxs/")};
    const auto absolute = folder.absolutePath();

    if (folder.mkpath(absolute)) {
        qDebug() << QString("Setting opentxs data folder to: %1").arg(absolute);
    } else {
        qFatal("Failed to create opentxs data folder");
    }

    auto& args = const_cast<ot::ArgList&>(ot_args_);
    args["qt"].emplace("true");
    args[OPENTXS_ARG_HOME].emplace(absolute.toStdString());
#ifdef DEFAULT_SYNC_SERVER
    args[OPENTXS_ARG_BLOCKCHAIN_SYNC].emplace(DEFAULT_SYNC_SERVER);
#endif
    auto parser = ArgumentParser{};
    parser.parse(argc, argv, args);

    if (parser.show_help_) { std::cout << parser.options() << '\n'; }

    return ot_args_;
}

namespace metier
{
constexpr auto seed_id_key{"seedid"};
constexpr auto nym_id_key{"nymid"};

struct OTWrap::Imp {
    struct EnabledChains {
        using Vector = std::set<ot::blockchain::Type>;

        auto count() const noexcept
        {
            ot::Lock lock(lock_);

            return enabled_.size();
        }
        auto get() const noexcept
        {
            auto output = BlockchainList{};
            auto copy = Vector{};
            {
                ot::Lock lock(lock_);
                copy = enabled_;
            }
            std::transform(
                std::begin(copy), std::end(copy), std::back_inserter(output), [
                ](const auto& in) -> auto { return static_cast<int>(in); });

            return output;
        }

        auto add(ot::blockchain::Type chain) noexcept
        {
            ot::Lock lock(lock_);
            enabled_.emplace(chain);
        }
        auto remove(ot::blockchain::Type chain) noexcept
        {
            ot::Lock lock(lock_);
            enabled_.erase(chain);
        }
        auto set(Vector&& in) noexcept
        {
            ot::Lock lock(lock_);
            std::swap(enabled_, in);
        }

        EnabledChains(Vector&& in) noexcept
            : enabled_(std::move(in))
        {
        }

    private:
        mutable std::mutex lock_{};
        Vector enabled_{};
    };

    const opentxs::api::Context& ot_;
    const opentxs::api::client::Manager& api_;
    const std::string seed_id_;
    const ot::OTNymID nym_id_;
    const int longest_seed_word_;
    mutable std::mutex lock_;
    EnabledChains enabled_chains_;
    std::unique_ptr<model::SeedType> seed_type_;
    std::map<int, std::unique_ptr<model::SeedLanguage>> seed_language_;
    std::map<int, std::unique_ptr<model::SeedSize>> seed_size_;
    std::unique_ptr<model::AccountList> account_list_;
    std::unique_ptr<model::Profile> profile_;
    std::map<ot::OTIdentifier, std::unique_ptr<model::AccountActivity>>
        account_activity_proxy_models_;
    std::unique_ptr<model::BlockchainChooser> mainnet_model_;

    template <typename OutputType, typename InputType>
    static auto transform(const InputType& data) noexcept -> OutputType
    {
        auto output = OutputType{};
        std::transform(
            data.begin(), data.end(), std::back_inserter(output), [
            ](const auto& in) -> auto {
                return std::make_pair(in.second, static_cast<int>(in.first));
            });
        std::sort(output.begin(), output.end());

        return output;
    }

    auto needNym() const noexcept { return 0 == api_.Wallet().LocalNymCount(); }
    auto needSeed() const noexcept { return api_.Storage().SeedList().empty(); }
    auto validateBlockchains() const noexcept
    {
        const auto& model =
            api_.UI().BlockchainSelection(ot::ui::Blockchains::All);
        auto row = model.First();
        auto accountCount = std::size_t{0};

        while (true) {
            if (false == row->Valid()) { break; }

            if (row->IsEnabled()) {
                const auto chain = row->Type();
                const auto accounts =
                    api_.Blockchain().AccountList(nym_id_, chain);
                accountCount += accounts.size();

                if (0 == accounts.size()) {
                    const auto prompt = std::string{"Creating a new "} +
                                        row->Name() + " account";
                    auto reason = api_.Factory().PasswordPrompt(prompt);
                    const auto id = api_.Blockchain().NewHDSubaccount(
                        nym_id_,
                        ot::BlockchainAccountType::BIP44,
                        chain,
                        reason);

                    if (id->empty()) { return false; }

                    ++accountCount;
                }
            }

            if (row->Last()) { break; }

            row = model.Next();
        }

        return 0 < accountCount;
    }
    auto validateNym() const noexcept
    {
        ot::Lock lock(lock_);
        auto postcondition = ScopeGuard{[this] {
            if (nym_id_->empty()) { return; }

            {
                auto& pointer =
                    const_cast<std::unique_ptr<model::AccountList>&>(
                        account_list_);

                if (!pointer) {

                    pointer = std::make_unique<model::AccountList>(
                        api_.UI().AccountListQt(nym_id_));

                    OT_ASSERT(pointer);

                    Ownership::Claim(pointer.get());
                }
            }
            {
                auto& pointer =
                    const_cast<std::unique_ptr<model::Profile>&>(profile_);

                if (!pointer) {

                    pointer = std::make_unique<model::Profile>(
                        api_.UI().ProfileQt(nym_id_));

                    OT_ASSERT(pointer);

                    Ownership::Claim(pointer.get());
                }
            }
        }};

        if (false == nym_id_->empty()) { return true; }

        auto& nymID = const_cast<ot::identifier::Nym&>(nym_id_.get());
        auto id = ot::String::Factory();
        bool notUsed{false};
        api_.Config().Check_str(
            ot::String::Factory(
                QGuiApplication::applicationName().toStdString()),
            ot::String::Factory(nym_id_key),
            id,
            notUsed);

        if (id->Exists()) {
            nymID.SetString(id->Get());

            return true;
        }

        const auto nymList = api_.Wallet().LocalNyms();

        if (1 == nymList.size()) {
            const auto& firstID = *nymList.begin();
            id->Set(firstID->str().c_str());
            const auto config = api_.Config().Set_str(
                ot::String::Factory(
                    QGuiApplication::applicationName().toStdString()),
                ot::String::Factory(nym_id_key),
                id,
                notUsed);

            if (false == config) { return false; }

            if (false == api_.Config().Save()) { return false; }

            nymID.SetString(id->Get());

            return true;
        }

        return false;
    }
    auto validateSeed() const noexcept
    {
        ot::Lock lock(lock_);

        if (false == seed_id_.empty()) { return true; }

        auto& seed = const_cast<std::string&>(seed_id_);
        auto id = ot::String::Factory();
        bool notUsed{false};
        api_.Config().Check_str(
            ot::String::Factory(
                QGuiApplication::applicationName().toStdString()),
            ot::String::Factory(seed_id_key),
            id,
            notUsed);

        if (id->Exists()) {
            seed = id->Get();

            return true;
        }

        const auto seedList = api_.Storage().SeedList();

        if (1 == seedList.size()) {
            const auto& firstID = seedList.begin()->first;
            id->Set(firstID.c_str());
            const auto config = api_.Config().Set_str(
                ot::String::Factory(
                    QGuiApplication::applicationName().toStdString()),
                ot::String::Factory(seed_id_key),
                id,
                notUsed);

            if (false == config) { return false; }

            if (false == api_.Config().Save()) { return false; }

            seed = id->Get();

            return true;
        }

        return false;
    }

    auto accountActivityModel(const ot::Identifier& id) noexcept
        -> model::AccountActivity*
    {
        ot::Lock lock{lock_};
        auto& map = account_activity_proxy_models_;

        if (auto it = map.find(id); map.end() != it) {

            return it->second.get();
        }

        auto [it, added] =
            map.try_emplace(id, std::make_unique<model::AccountActivity>());

        OT_ASSERT(added);

        auto& pModel = it->second;

        OT_ASSERT(pModel);

        auto& model = *(it->second);
        model.setSourceModel(api_.UI().AccountActivityQt(nym_id_, id));
        auto* output = pModel.get();
        Ownership::Claim(output);

        return output;
    }
    auto createNym(QString alias) noexcept -> void
    {
        ot::Lock lock(lock_);
        auto success{false};
        auto& id = const_cast<ot::identifier::Nym&>(nym_id_.get());
        auto postcondition = ScopeGuard{[&]() {
            if (false == success) { id.Release(); }
        }};
        const auto reason =
            api_.Factory().PasswordPrompt("Generate a new Metier identity");

        OT_ASSERT(false == seed_id_.empty());

        const auto pNym =
            api_.Wallet().Nym(reason, alias.toStdString(), {seed_id_, 0});

        if (!pNym) { return; }

        const auto& nym = *pNym;
        bool notUsed{false};
        const auto config = api_.Config().Set_str(
            ot::String::Factory(
                QGuiApplication::applicationName().toStdString()),
            ot::String::Factory(nym_id_key),
            ot::String::Factory(nym.ID().str()),
            notUsed);

        if (false == config) { return; }
        if (false == api_.Config().Save()) { return; }

        id.Assign(nym.ID());
    }
    auto createNewSeed(
        const int type,
        const int lang,
        const int strength) noexcept -> QStringList
    {
        ot::Lock lock(lock_);
        auto success{false};
        auto& id = const_cast<std::string&>(seed_id_);
        auto postcondition = ScopeGuard{[&]() {
            if (false == success) { id = {}; }
        }};
        const auto& seeds = api_.Seeds();

        OT_ASSERT(id.empty());
        OT_ASSERT(seeds.DefaultSeed().empty());

        const auto invalid = [](const int in) -> auto
        {
            return (0 > in) || (std::numeric_limits<std::uint8_t>::max() < in);
        };

        if (invalid(type) || invalid(lang) || invalid(strength)) { return {}; }

        auto reason =
            api_.Factory().PasswordPrompt("Generate a new Metier wallet seed");
        id = seeds.NewSeed(
            static_cast<ot::crypto::SeedStyle>(static_cast<std::uint8_t>(type)),
            static_cast<ot::crypto::Language>(static_cast<std::uint8_t>(lang)),
            static_cast<ot::crypto::SeedStrength>(
                static_cast<std::size_t>(strength)),
            reason);

        if (id.empty()) { return {}; }

        auto notUsed{false};
        const auto config = api_.Config().Set_str(
            ot::String::Factory(
                QGuiApplication::applicationName().toStdString()),
            ot::String::Factory(seed_id_key),
            ot::String::Factory(id),
            notUsed);

        if (false == config) { return {}; }
        if (false == api_.Config().Save()) { return {}; }

        success = true;
        const auto words = QString{seeds.Words(reason, id).c_str()};

        return words.split(' ', Qt::SkipEmptyParts);
    }
    auto getRecoveryWords() -> QStringList
    {
        ot::Lock lock(lock_);
        const auto& seeds = api_.Seeds();
        const auto reason =
            api_.Factory().PasswordPrompt("Loading recovery words for backup");
        const auto words = QString{seeds.Words(reason, seed_id_).c_str()};

        return words.split(' ', Qt::SkipEmptyParts);
    }
    auto importSeed(int type, int lang, QString input) -> void
    {
        ot::Lock lock(lock_);
        auto success{false};
        auto& id = const_cast<std::string&>(seed_id_);
        auto postcondition = ScopeGuard{[&]() {
            if (false == success) { id = {}; }
        }};
        const auto& seeds = api_.Seeds();

        OT_ASSERT(id.empty());
        OT_ASSERT(seeds.DefaultSeed().empty());

        auto reason =
            api_.Factory().PasswordPrompt("Import a Metier wallet seed");
        const auto words = api_.Factory().SecretFromText(input.toStdString());
        const auto passphrase = api_.Factory().Secret(0);
        id = seeds.ImportSeed(
            words,
            passphrase,
            static_cast<ot::crypto::SeedStyle>(static_cast<std::uint8_t>(type)),
            static_cast<ot::crypto::Language>(static_cast<std::uint8_t>(lang)),
            reason);

        if (id.empty()) { return; }

        auto notUsed{false};
        const auto config = api_.Config().Set_str(
            ot::String::Factory(
                QGuiApplication::applicationName().toStdString()),
            ot::String::Factory(seed_id_key),
            ot::String::Factory(id),
            notUsed);

        if (false == config) { return; }
        if (false == api_.Config().Save()) { return; }

        success = true;
    }
    auto seedLanguageModel(const int type) -> model::SeedLanguage*
    {
        ot::Lock lock(lock_);
        {
            auto it = seed_language_.find(type);

            if (seed_language_.end() != it) { return it->second.get(); }
        }

        if ((0 > type) || (std::numeric_limits<std::uint8_t>::max() < type)) {

            return nullptr;
        }

        const auto style =
            static_cast<ot::crypto::SeedStyle>(static_cast<std::uint8_t>(type));

        auto [it, added] = seed_language_.try_emplace(
            type,
            std::make_unique<model::SeedLanguage>(
                transform<model::SeedLanguage::Data>(
                    api_.Seeds().AllowedLanguages(style))));
        auto& pModel = it->second;

        OT_ASSERT(pModel);

        auto output = pModel.get();
        Ownership::Claim(output);

        return output;
    }
    auto seedSizeModel(const int type) -> model::SeedSize*
    {
        ot::Lock lock(lock_);
        {
            auto it = seed_size_.find(type);

            if (seed_size_.end() != it) { return it->second.get(); }
        }

        if ((0 > type) || (std::numeric_limits<std::uint8_t>::max() < type)) {

            return nullptr;
        }

        const auto style =
            static_cast<ot::crypto::SeedStyle>(static_cast<std::uint8_t>(type));

        auto [it, added] = seed_size_.try_emplace(
            type,
            std::make_unique<model::SeedSize>(transform<model::SeedSize::Data>(
                api_.Seeds().AllowedSeedStrength(style))));
        auto& pModel = it->second;

        OT_ASSERT(pModel);

        auto output = pModel.get();
        Ownership::Claim(output);

        return output;
    }
    auto seedWordValidator(const int type, const int lang)
        -> const opentxs::ui::SeedValidator*
    {
        const auto style =
            static_cast<ot::crypto::SeedStyle>(static_cast<std::uint8_t>(type));
        const auto language =
            static_cast<ot::crypto::Language>(static_cast<std::uint8_t>(lang));

        return api_.UI().SeedValidator(style, language);
    }
    auto wordCount(const int type, const int strength) -> int
    {
        const auto output = api_.Seeds().WordCount(
            static_cast<ot::crypto::SeedStyle>(static_cast<std::uint8_t>(type)),
            static_cast<ot::crypto::SeedStrength>(
                static_cast<std::size_t>(strength)));

        return static_cast<int>(output);
    }

    Imp(QGuiApplication& parent, OTWrap& me, int& argc, char** argv)
        : ot_(ot::InitContext(make_args(parent, argc, argv)))
        , api_(ot_.StartClient(ot_args_, 0))
        , seed_id_()
        , nym_id_(api_.Factory().NymID())
        , longest_seed_word_([&]() -> auto {
            const auto& api = api_.Seeds();
            auto output = int{};
            const auto types = api.AllowedSeedTypes();

            for (const auto& [style, description] : types) {
                const auto langs = api.AllowedLanguages(style);

                for (const auto& [lang, name] : langs) {
                    output = std::max(
                        output, static_cast<int>(api.LongestWord(style, lang)));
                }
            }

            return output;
        }())
        , lock_()
        , enabled_chains_([&] {
            auto* full =
                api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::All);
            auto* main =
                api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::Main);
            auto* test =
                api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::Test);

            OT_ASSERT(nullptr != full);
            OT_ASSERT(nullptr != main);
            OT_ASSERT(nullptr != test);

            using Model = ot::ui::BlockchainSelectionQt;
            connect(full, &Model::enabledChanged, &me, &OTWrap::chainsChanged);
            connect(full, &Model::chainEnabled, [&](const int chain) {
                enabled_chains_.add(static_cast<ot::blockchain::Type>(chain));
            });
            connect(full, &Model::chainDisabled, [&](const int chain) {
                enabled_chains_.remove(
                    static_cast<ot::blockchain::Type>(chain));
            });

            return api_.Blockchain().EnabledChains();
        }())
        , seed_type_(std::make_unique<model::SeedType>(
              transform<model::SeedType::Data>(
                  api_.Seeds().AllowedSeedTypes())))
        , seed_language_()
        , seed_size_()
        , account_list_()
        , profile_()
        , account_activity_proxy_models_()
        , mainnet_model_(std::make_unique<model::BlockchainChooser>(
              api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::Main)))
    {
        OT_ASSERT(seed_type_);

        Ownership::Claim(mainnet_model_.get());
        Ownership::Claim(seed_type_.get());
#ifdef METIER_DEFAULT_BLOCKCHAIN
        api_.Blockchain().Enable(util::convert(METIER_DEFAULT_BLOCKCHAIN));
#endif
    }
};
}  // namespace metier
