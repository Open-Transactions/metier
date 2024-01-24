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
    Api& parent_;
    QGuiApplication& qt_parent_;

public:
    enum class State { init, have_seed, have_nym, run };

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

    using SeedLanguageMap =
        std::map<ot::crypto::SeedStyle, std::unique_ptr<model::SeedLanguage>>;
    using SeedSizeMap =
        std::map<ot::crypto::SeedStyle, std::unique_ptr<model::SeedSize>>;
    using Lock = std::unique_lock<std::mutex>;

    PasswordCallback callback_;
    opentxs::PasswordCaller caller_;
    const opentxs::api::Context& ot_;
    const opentxs::api::session::Client& api_;
    const opentxs::crypto::SeedID seed_id_;
    const int longest_seed_word_;
    const SeedLanguageMap seed_language_;
    const SeedSizeMap seed_size_;
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

    auto check_chains(int count) const noexcept -> void;
    auto enableDefaultChain() const noexcept -> bool;
    auto identityManager() const noexcept -> ot::ui::IdentityManagerQt*;
    auto needNym() const noexcept;
    auto rescanBlockchain(int chain) -> void;
    auto seedManager() const noexcept -> ot::ui::SeedTreeQt*;
    auto validateBlockchains() const noexcept -> bool;
    auto validateNym() const noexcept -> bool;
    auto validateSeed() const noexcept -> bool;

    auto blockchainTypeToAccountID(int chain) noexcept -> QString;
    auto chain_is_disabled(int chain) noexcept -> void;
    auto chain_is_enabled(int chain) noexcept -> void;
    auto createNym(QString alias) noexcept -> void;
    auto createNewSeed(
        const int type,
        const int lang,
        const int strength) noexcept -> QStringList;
    auto getRecoveryWords() -> QStringList;
    auto importSeed(
        int type,
        int lang,
        const QString& input,
        const QString& password) -> void;
    auto seedLanguageModel(const int type) -> model::SeedLanguage*;
    auto seedSizeModel(const int type) -> model::SeedSize*;
    auto seedWordValidator(const int type, const int lang)
        -> const opentxs::ui::SeedValidator*;
    auto wordCount(const int type, const int strength) -> int;

    Imp(QGuiApplication& parent, App& app, Api& me, int& argc, char** argv);
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;

    ~Imp();

private:
    auto make_accounts(const ot::blockchain::Type chain) const noexcept -> bool;
    auto receive_message(
        void* socket,
        ot::network::zeromq::Message& message) noexcept -> bool;
    auto rpc(void* socket, zmq::Message&& in) const noexcept -> void;
    auto run_zmq() noexcept -> void;
};
}  // namespace metier::common
