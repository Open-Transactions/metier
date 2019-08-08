// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "otwrap.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <algorithm>
#include <map>
#include <mutex>
#include <set>
#include <string>

#include "models/accountactivity.hpp"
#include "models/blockchainchooser.hpp"
#include "util/scopeguard.hpp"

namespace ot = opentxs;

static const auto ot_args_ = ot::ArgList{};

auto make_args(QApplication& parent) noexcept -> const ot::ArgList&;
auto make_args(QApplication& parent) noexcept -> const ot::ArgList&
{
    parent.setOrganizationDomain("opentransactions.org");
    parent.setApplicationName("metier");
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

    return ot_args_;
}

namespace metier
{
constexpr auto seed_id_key{"seedid"};
constexpr auto nym_id_key{"nymid"};

struct OTWrap::Imp {
    struct EnabledChains {
        using Vector = std::set<ot::blockchain::Type>;

        auto get() const noexcept
        {
            auto output = EnabledBlockchains{};
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

        auto set(Vector&& in) noexcept
        {
            ot::Lock lock(lock_);
            std::swap(enabled_, in);
        }

    private:
        mutable std::mutex lock_{};
        Vector enabled_{};
    };

    const opentxs::api::Context& ot_;
    const opentxs::api::client::Manager& api_;
    const opentxs::ui::BlockchainSelection& selector_model_native_;
    const std::string seed_id_;
    const ot::OTNymID nym_id_;
    mutable std::mutex lock_;
    mutable EnabledChains enabled_chains_;
    std::unique_ptr<model::BlockchainChooser> blockchain_chooser_mainnet_;
    std::unique_ptr<model::BlockchainChooser> blockchain_chooser_testnet_;
    std::map<ot::OTIdentifier, std::unique_ptr<model::AccountActivity>>
        account_activity_proxy_models_;

    auto scanBlockchains() const noexcept -> std::pair<int, int>
    {
        auto enabledVector = EnabledChains::Vector{};
        auto output = std::pair<int, int>{};
        auto& [enabled, longest] = output;
        const auto& model = selector_model_native_;
        auto row = model.First();

        while (true) {
            if (false == row->Valid()) { break; }

            if (row->IsEnabled()) {
                ++enabled;
                enabledVector.emplace(row->Type());
            }

            longest = std::max(longest, static_cast<int>(row->Name().size()));

            if (row->Last()) { break; }

            row = model.Next();
        }

        enabled_chains_.set(std::move(enabledVector));

        return output;
    }
    auto needNym() const noexcept { return 0 == api_.Wallet().LocalNymCount(); }
    auto needSeed() const noexcept { return api_.Storage().SeedList().empty(); }
    auto validateBlockchains() const noexcept
    {
        auto enabledVector = EnabledChains::Vector{};
        const auto& model = selector_model_native_;
        auto row = model.First();
        auto accountCount = std::size_t{0};

        while (true) {
            if (false == row->Valid()) { break; }

            if (row->IsEnabled()) {
                const auto chain = row->Type();
                enabledVector.emplace(chain);
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

        enabled_chains_.set(std::move(enabledVector));

        return 0 < accountCount;
    }
    auto validateNym() const noexcept
    {
        ot::Lock lock(lock_);

        if (false == nym_id_->empty()) { return true; }

        auto& nymID = const_cast<ot::identifier::Nym&>(nym_id_.get());
        auto id = ot::String::Factory();
        bool notUsed{false};
        api_.Config().Check_str(
            ot::String::Factory(QApplication::applicationName().toStdString()),
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
                    QApplication::applicationName().toStdString()),
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
            ot::String::Factory(QApplication::applicationName().toStdString()),
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
                    QApplication::applicationName().toStdString()),
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

        return pModel.get();
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
            ot::String::Factory(QApplication::applicationName().toStdString()),
            ot::String::Factory(nym_id_key),
            ot::String::Factory(nym.ID().str()),
            notUsed);

        if (false == config) { return; }
        if (false == api_.Config().Save()) { return; }

        id.Assign(nym.ID());
    }
    auto createNewSeed() noexcept -> void
    {
        ot::Lock lock(lock_);
        auto success{false};
        auto& id = const_cast<std::string&>(seed_id_);
        auto postcondition = ScopeGuard{[&]() {
            if (false == success) { id = {}; }
        }};
        auto reason =
            api_.Factory().PasswordPrompt("Generate a new Metier wallet seed");
        auto index = std::uint32_t{0};
        auto notUsed{false};
        api_.Seeds().Seed(id, index, reason);

        if (id.empty()) { return; }

        const auto config = api_.Config().Set_str(
            ot::String::Factory(QApplication::applicationName().toStdString()),
            ot::String::Factory(seed_id_key),
            ot::String::Factory(id),
            notUsed);

        if (false == config) { return; }
        if (false == api_.Config().Save()) { return; }

        success = true;
    }

    Imp(QApplication& parent, OTWrap& me)
        : ot_(ot::InitContext(make_args(parent)))
        , api_(ot_.StartClient(ot_args_, 0))
        , selector_model_native_(api_.UI().BlockchainSelection())
        , seed_id_()
        , nym_id_(api_.Factory().NymID())
        , lock_()
        , enabled_chains_()
        , blockchain_chooser_mainnet_(
              std::make_unique<model::BlockchainChooser>(me, false))
        , blockchain_chooser_testnet_(
              std::make_unique<model::BlockchainChooser>(me, true))
        , account_activity_proxy_models_()
    {
        auto* blockchainChooser = api_.UI().BlockchainSelectionQt();
        blockchain_chooser_mainnet_->setSourceModel(blockchainChooser);
        blockchain_chooser_testnet_->setSourceModel(blockchainChooser);
        selector_model_native_.SetCallback([&me]() { me.checkChainCount(); });
        // TODO clear the above callback in the destructor for this class
    }
};
}  // namespace metier
