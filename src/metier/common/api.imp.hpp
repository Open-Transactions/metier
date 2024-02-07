// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opentxs/opentxs.hpp>
#include <QList>
#include <QString>
#include <QStringList>
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <functional>
#include <future>
#include <iterator>
#include <memory>
#include <mutex>
#include <string_view>
#include <thread>
#include <utility>

#include "metier/common/api.hpp"
#include "metier/common/model/seedlang.hpp"
#include "metier/common/model/seedsize.hpp"
#include "metier/common/passwordcallback.hpp"

class QGuiApplication;

namespace metier
{
namespace common
{
namespace model
{
class SeedType;
}  // namespace model

class App;
}  // namespace common
}  // namespace metier

namespace opentxs
{
namespace ui
{
class IdentityManagerQt;
class SeedTreeQt;
class SeedValidator;
}  // namespace ui
}  // namespace opentxs

namespace ot = opentxs;

namespace metier::common
{
using namespace std::literals;

constexpr auto seed_id_key{"seedid"};
constexpr auto nym_id_key{"nymid"};

auto make_args(QGuiApplication& parent, int& argc, char** argv) noexcept
    -> const ot::Options&;
auto ot_args() -> const ot::Options&;

auto ready(bool complete = false) noexcept -> std::shared_future<void>;

namespace zmq = opentxs::network::zeromq;

struct Api::Imp {
private:
    Api* parent_;
    QGuiApplication& qt_parent_;
    PasswordCallback callback_;
    opentxs::PasswordCaller caller_;

public:
    enum class State { init, have_seed, have_nym, run };

    using SeedLanguageMap =
        std::map<ot::crypto::SeedStyle, std::unique_ptr<model::SeedLanguage>>;
    using SeedSizeMap =
        std::map<ot::crypto::SeedStyle, std::unique_ptr<model::SeedSize>>;

    const opentxs::api::Context& ot_;
    const opentxs::api::session::Client& api_;
    const opentxs::crypto::SeedID seed_id_;
    const int longest_seed_word_;
    const SeedLanguageMap seed_language_;
    const SeedSizeMap seed_size_;

    virtual auto check_chains(int count) const noexcept -> void;
    virtual auto enableDefaultChain() const noexcept -> bool;
    virtual auto enabledChainCount() const noexcept -> std::size_t;
    virtual auto enabledChainList() const noexcept -> BlockchainList;
    virtual auto identityManager() const noexcept -> ot::ui::IdentityManagerQt*;
    virtual auto needNym() const noexcept -> bool;
    virtual auto rescanBlockchain(int chain) -> void;
    virtual auto seedManager() const noexcept -> ot::ui::SeedTreeQt*;
    virtual auto seedTypeModel() const -> model::SeedType*;
    virtual auto state(State value) noexcept -> void;
    virtual auto validateBlockchains() const noexcept -> bool;
    virtual auto validateNym() const noexcept -> bool;
    virtual auto validateSeed() const noexcept -> bool;
    virtual auto waitForSeedBackup() const noexcept -> bool;

    virtual auto blockchainTypeToAccountID(int chain) noexcept -> QString;
    virtual auto chain_is_disabled(int chain) noexcept -> void;
    virtual auto chain_is_enabled(int chain) noexcept -> void;
    virtual auto createNym(QString alias) noexcept -> void;
    virtual auto createNewSeed(
        const int type,
        const int lang,
        const int strength) noexcept -> QStringList;
    virtual auto getRecoveryWords() -> QStringList;
    virtual auto importSeed(
        int type,
        int lang,
        const QString& input,
        const QString& password) -> void;
    virtual auto init(Api* me) -> void;
    virtual auto seedLanguageModel(const int type) -> model::SeedLanguage*;
    virtual auto seedSizeModel(const int type) -> model::SeedSize*;
    virtual auto seedWordValidator(const int type, const int lang)
        -> const opentxs::ui::SeedValidator*;
    virtual auto state() const noexcept -> State;
    virtual auto waitForSeedBackup(bool value) noexcept -> void;
    virtual auto wordCount(const int type, const int strength) -> int;

    Imp(QGuiApplication& parent, App& app, int& argc, char** argv);
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;

    virtual ~Imp();

private:
    using Lock = std::unique_lock<std::mutex>;

    struct EnabledChains {
        using Vector = opentxs::Set<ot::blockchain::Type>;
        using Lock = std::unique_lock<std::mutex>;

        auto count() const noexcept
        {
            auto lock = Lock{lock_};

            return enabled_.size();
        }
        auto get() const noexcept
        {
            auto output = BlockchainList{};
            auto copy = Vector{};
            {
                auto lock = Lock{lock_};
                copy = enabled_;
            }
            std::transform(
                std::begin(copy),
                std::end(copy),
                std::back_inserter(output),
                [](const auto& in) -> auto { return static_cast<int>(in); });

            return output;
        }

        auto add(ot::blockchain::Type chain) noexcept
        {
            auto lock = Lock{lock_};
            enabled_.emplace(chain);
        }
        auto remove(ot::blockchain::Type chain) noexcept
        {
            auto lock = Lock{lock_};
            enabled_.erase(chain);
        }
        auto set(Vector&& in) noexcept
        {
            auto lock = Lock{lock_};
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

    mutable std::mutex lock_;
    mutable std::atomic_bool have_nym_;
    mutable std::atomic_bool wait_for_seed_backup_;
    mutable std::atomic_bool run_zmq_;
    mutable std::atomic<State> state_;
    EnabledChains enabled_chains_;
    std::unique_ptr<model::SeedType> seed_type_;
    void* zmq_socket_;
    std::thread zmq_thread_;

    template <typename OutputType, typename InputType>
    static auto transform(const InputType& data) noexcept -> OutputType;

    virtual auto make_accounts(const ot::blockchain::Type chain) const noexcept
        -> bool;
    auto receive_message(
        void* socket,
        ot::network::zeromq::Message& message) noexcept -> bool;
    auto rpc(void* socket, zmq::Message&& in) const noexcept -> void;
    auto run_zmq() noexcept -> void;
};
}  // namespace metier::common
