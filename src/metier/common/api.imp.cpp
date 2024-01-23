// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/common/api.imp.hpp"  // IWYU pragma: associated

#include <opentxs/Qt.hpp>
#include <opentxs/opentxs.hpp>
#include <zmq.h>
#include <QDebug>
#include <QDir>
#include <QGuiApplication>
#include <QStandardPaths>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstring>
#include <future>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <thread>

#include "metier/common/app.hpp"
#include "metier/common/claim.hpp"
#include "metier/common/cli/endpoint.hpp"
#include "metier/common/convertblockchain.hpp"
#include "metier/common/custom.hpp"
#include "metier/common/model/seedlang.hpp"
#include "metier/common/model/seedsize.hpp"
#include "metier/common/model/seedtype.hpp"
#include "metier/common/scopeguard.hpp"

namespace metier::common
{
auto make_args(QGuiApplication& parent, int& argc, char** argv) noexcept
    -> const ot::Options&
{
    parent.setOrganizationDomain(Api::Domain());
    parent.setApplicationName(Api::Name());
    auto& args = const_cast<ot::Options&>(ot_args());
    args.ParseCommandLine(argc, argv);

    if (args.Home().empty()) {
        auto path =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        if (path.isEmpty()) { qFatal("Invalid app data folder"); }

        auto folder = QDir{path.append("/opentxs/")};
        const auto absolute = folder.absolutePath();

        if (false == folder.mkpath(absolute)) {
            qFatal("Failed to create opentxs data folder");
        }

        args.SetHome(absolute.toStdString());
    }

    qDebug() << QString("Using %1 for opentxs data folder")
                    .arg(args.Home().string().c_str());
    args.SetBlockchainProfile(ot::BlockchainProfile::desktop);

    for (const auto* endpoint : metier::common::SeedEndpoints()) {
        args.AddBlockchainSyncServer(endpoint);
    }

    args.SetQtRootObject(&parent);
    args.SetIpv4ConnectionMode(ot::ConnectionMode::on);
    args.SetIpv6ConnectionMode(ot::ConnectionMode::automatic);

    return ot_args();
}

auto ot_args() -> const ot::Options&
{
    static auto ot_args = ot::Options{};
    return ot_args;
}

auto ready(bool complete) noexcept -> std::shared_future<void>
{
    static auto mutex = std::mutex{};
    auto lock = std::lock_guard<std::mutex>{mutex};
    static auto promise = std::promise<void>{};
    static auto future = std::shared_future<void>{promise.get_future()};

    if (complete) { promise.set_value(); }

    return future;
}
}  // namespace metier::common

namespace metier::common
{
template <typename OutputType, typename InputType>
auto Api::Imp::transform(const InputType& data) noexcept -> OutputType
{
    auto output = OutputType{};
    std::transform(
        data.begin(),
        data.end(),
        std::back_inserter(output),
        [](const auto& in) -> auto {
            return std::make_pair(
                std::string{in.second}, static_cast<int>(in.first));
        });
    std::sort(output.begin(), output.end());

    return output;
}
}  // namespace metier::common

namespace metier::common
{
Api::Imp::Imp(
    QGuiApplication& parent,
    App& app,
    Api& me,
    int& argc,
    char** argv)
    : parent_(me)
    , qt_parent_(parent)
    , callback_(app)
    , caller_()
    , ot_(ot::InitContext(
          make_args(parent, argc, argv),
          [this]() -> auto {
              caller_.SetCallback(&callback_);
              return &caller_;
          }()))
    , api_(ot_.StartClientSession(ot_args(), 0))
    , seed_id_()
    , longest_seed_word_([&]() -> auto {
        const auto& api = api_.Crypto().Seed();
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
    , seed_language_([this] {
        auto out = SeedLanguageMap{};
        const auto& api = api_.Crypto().Seed();
        const auto types = api.AllowedSeedTypes();

        for (const auto& [style, description] : types) {
            auto [it, added] = out.try_emplace(
                style,
                std::make_unique<model::SeedLanguage>(
                    &qt_parent_,
                    transform<model::SeedLanguage::Data>(
                        api_.Crypto().Seed().AllowedLanguages(style))));
            auto& pModel = it->second;

            assert(pModel);

            Ownership::Claim(pModel.get());
        }

        return out;
    }())
    , seed_size_([this] {
        auto out = SeedSizeMap{};
        const auto& api = api_.Crypto().Seed();
        const auto types = api.AllowedSeedTypes();

        for (const auto& [style, description] : types) {
            auto [it, added] = out.try_emplace(
                style,
                std::make_unique<model::SeedSize>(
                    &qt_parent_,
                    transform<model::SeedSize::Data>(
                        api_.Crypto().Seed().AllowedSeedStrength(style))));
            auto& pModel = it->second;

            assert(pModel);

            Ownership::Claim(pModel.get());
        }

        return out;
    }())
    , lock_()
    , have_nym_(false)
    , wait_for_seed_backup_(false)
    , run_zmq_(true)
    , state_(State::init)
    , enabled_chains_([&] {
        auto* full = api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::All);
        auto* main = api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::Main);
        auto* test = api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::Test);

        assert(nullptr != full);
        assert(nullptr != main);
        assert(nullptr != test);

        using Model = ot::ui::BlockchainSelectionQt;
        connect(full, &Model::enabledChanged, &parent_, &Api::checkChains);
        connect(full, &Model::chainEnabled, &parent_, &Api::chainIsEnabled);
        connect(full, &Model::chainDisabled, &parent_, &Api::chainIsDisabled);

        return api_.Network().Blockchain().EnabledChains();
    }())
    , seed_type_(std::make_unique<model::SeedType>(
          &parent,
          transform<model::SeedType::Data>(
              api_.Crypto().Seed().AllowedSeedTypes())))
    , zmq_socket_(::zmq_socket(api_.Network().ZeroMQ().Context(), ZMQ_ROUTER))
    , zmq_thread_(&Imp::run_zmq, this)
{
    assert(seed_type_);

    Ownership::Claim(seed_type_.get());
    ready(true);
}

auto Api::Imp::blockchainTypeToAccountID(int chain) noexcept -> QString
{
    const auto active = identityManager()->getActiveNym();

    if (active.isEmpty()) { return {}; }

    const auto nymID = api_.Factory().NymIDFromBase58(active.toStdString());

    if (nymID.empty()) { return {}; }

    const auto& account =
        api_.Crypto().Blockchain().Account(nymID, convert(chain));

    return QString::fromStdString(account.AccountID().asBase58(api_.Crypto()));
}

auto Api::Imp::chain_is_disabled(int chain) noexcept -> void
{
    enabled_chains_.remove(static_cast<ot::blockchain::Type>(chain));
}

auto Api::Imp::chain_is_enabled(int chain) noexcept -> void
{
    enabled_chains_.add(static_cast<ot::blockchain::Type>(chain));
}

auto Api::Imp::check_chains(int count) const noexcept -> void
{
    validateBlockchains();
    Q_EMIT parent_.chainsChanged(count);
}

auto Api::Imp::createNym(QString alias) noexcept -> void
{
    ready().get();
    auto lock = Lock{lock_};
    const auto reason =
        api_.Factory().PasswordPrompt("Generate a new Metier identity");

    assert(false == seed_id_.empty());

    const auto pNym = api_.Wallet().Nym(
        {api_.Factory(), seed_id_, 0}, reason, alias.toStdString());

    if (!pNym) { qFatal("Failed to create nym"); }

    const auto& nym = *pNym;
    const auto& config = api_.Config();
    const auto section = QGuiApplication::applicationName().toStdString();
    const auto rc = config.WriteString(
        section, nym_id_key, nym.ID().asBase58(api_.Crypto()));

    if (false == rc) { qFatal("Failed to update configuration"); }

    if (false == config.Save()) { qFatal("Failed to save config file"); }

    identityManager()->setActiveNym(
        QString::fromStdString(nym.ID().asBase58(api_.Crypto())));
    have_nym_ = true;
}

auto Api::Imp::createNewSeed(
    const int type,
    const int lang,
    const int strength) noexcept -> QStringList
{
    ready().get();
    wait_for_seed_backup_ = true;
    auto lock = Lock{lock_};
    auto success{false};
    auto& id = const_cast<opentxs::crypto::SeedID&>(seed_id_);
    auto postcondition = ScopeGuard{[&]() {
        if (false == success) { id = {}; }
    }};
    const auto& seeds = api_.Crypto().Seed();

    assert(id.empty());
    assert(0 == seeds.DefaultSeed().second);

    const auto invalid = [](const int in) -> auto {
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

    const auto& config = api_.Config();
    const auto section = QGuiApplication::applicationName().toStdString();
    const auto rc =
        config.WriteString(section, seed_id_key, id.asBase58(api_.Crypto()));

    if (false == rc) { qFatal("Failed to update configuration"); }

    if (false == config.Save()) { qFatal("Failed to save config file"); }

    success = true;
    const auto words = QString{seeds.Words(id, reason).c_str()};

    return words.split(' ', Qt::SkipEmptyParts);
}

auto Api::Imp::getRecoveryWords() -> QStringList
{
    ready().get();
    auto lock = Lock{lock_};
    const auto& seeds = api_.Crypto().Seed();
    const auto reason =
        api_.Factory().PasswordPrompt("Loading recovery words for backup");
    const auto words = QString{seeds.Words(seed_id_, reason).c_str()};

    return words.split(' ', Qt::SkipEmptyParts);
}

auto Api::Imp::importSeed(
    int type,
    int lang,
    const QString& input,
    const QString& password) -> void
{
    ready().get();
    auto lock = Lock{lock_};
    auto success{false};
    auto& id = const_cast<opentxs::crypto::SeedID&>(seed_id_);
    auto postcondition = ScopeGuard{[&]() {
        if (false == success) { id = {}; }
    }};
    const auto& seeds = api_.Crypto().Seed();

    assert(id.empty());
    assert(0u == seeds.DefaultSeed().second);

    auto reason = api_.Factory().PasswordPrompt("Import a Metier wallet seed");
    const auto words = api_.Factory().SecretFromText(input.toStdString());
    const auto passphrase =
        api_.Factory().SecretFromText(password.toStdString());
    id = seeds.ImportSeed(
        words,
        passphrase,
        static_cast<ot::crypto::SeedStyle>(static_cast<std::uint8_t>(type)),
        static_cast<ot::crypto::Language>(static_cast<std::uint8_t>(lang)),
        reason);

    if (id.empty()) { return; }

    const auto& config = api_.Config();
    const auto section = QGuiApplication::applicationName().toStdString();
    const auto rc =
        config.WriteString(section, seed_id_key, id.asBase58(api_.Crypto()));

    if (false == rc) { qFatal("Failed to update configuration"); }

    if (false == config.Save()) { qFatal("Failed to save config file"); }

    success = true;
}

auto Api::Imp::enableDefaultChain() const noexcept -> bool
{
    auto output{true};
    const auto chains = DefaultBlockchains();

    if (0u == chains.size()) { return false; }

    for (const auto chain : chains) {
        output &= api_.Network().Blockchain().Enable(chain);
    }

    return output;
}

auto Api::Imp::identityManager() const noexcept -> ot::ui::IdentityManagerQt*
{
    static auto* model = api_.UI().IdentityManagerQt();

    return model;
}

auto Api::Imp::make_accounts(const ot::blockchain::Type chain) const noexcept
    -> bool
{
    using Chain = ot::blockchain::Type;
    using Protocol = ot::blockchain::crypto::HDProtocol;
    const auto nymID = api_.Factory().NymIDFromBase58(
        identityManager()->getActiveNym().toStdString());
    const auto want = [&] {
        auto out = std::set<Protocol>{};
        out.emplace(Protocol::BIP_44);

        if (ot::blockchain::has_segwit(chain)) {
            out.emplace(Protocol::BIP_49);
            out.emplace(Protocol::BIP_84);
        }

        return out;
    }();
    const auto have = [&] {
        auto out = std::set<Protocol>{};
        const auto& account = api_.Crypto().Blockchain().Account(nymID, chain);
        using enum opentxs::blockchain::crypto::SubaccountType;

        for (const auto& hd : account.GetSubaccounts(HD)) {
            out.emplace(hd.asDeterministic().asHD().Standard());
        }

        return out;
    }();
    const auto need = [&] {
        auto out = std::vector<Protocol>{};
        std::set_difference(
            want.begin(),
            want.end(),
            have.begin(),
            have.end(),
            std::back_inserter(out));

        return out;
    }();

    for (const auto& type : need) {
        const auto prompt = std::string{"Creating a new "}
                                .append(print(chain))
                                .append(" account");
        const auto reason = api_.Factory().PasswordPrompt(prompt);
        const auto id = [&] {
            if ((Chain::PKT == chain) && (Protocol::BIP_84 == type)) {
                // TODO only do this if the primary seed is a pktwallet type

                return api_.Crypto().Blockchain().NewHDSubaccount(
                    nymID, type, Chain::Bitcoin, chain, reason);
            } else {

                return api_.Crypto().Blockchain().NewHDSubaccount(
                    nymID, type, chain, reason);
            }
        }();

        if (id.empty()) { return false; }
    }

    return true;
}

auto Api::Imp::needNym() const noexcept
{
    ready().get();

    return 0 == api_.Wallet().LocalNymCount();
}

auto Api::Imp::receive_message(
    void* socket,
    ot::network::zeromq::Message& message) noexcept -> bool
{
    auto receiving{true};

    while (receiving) {
        auto& frame = message.AddFrame();
        const bool received =
            (-1 != ::zmq_msg_recv(frame, socket, ZMQ_DONTWAIT));

        if (false == received) {
            auto zerr = ::zmq_errno();

            if (EAGAIN == zerr) {
                qInfo() << "zmq_msg_recv returns EAGAIN. This should never "
                           "happen.";
            } else {
                qInfo() << ::zmq_strerror(::zmq_errno());
            }

            return false;
        }

        int option{0};
        std::size_t optionBytes{sizeof(option)};

        const bool haveOption =
            (-1 !=
             ::zmq_getsockopt(socket, ZMQ_RCVMORE, &option, &optionBytes));

        if (false == haveOption) {
            qInfo() << "Failed to check socket options error:\n"
                    << ::zmq_strerror(::zmq_errno());

            return false;
        }

        if (optionBytes != sizeof(option)) { std::terminate(); }

        if (1 != option) { receiving = false; }
    }

    return true;
}

auto Api::Imp::rescanBlockchain(int chain) -> void
{
    static const auto valid = [] {
        auto output = std::set<int>{};
        const auto& input = ot::blockchain::supported_chains();
        std::transform(
            input.begin(),
            input.end(),
            std::inserter(output, output.end()),
            [](const auto in) { return convert(in); });

        return output;
    }();

    if (0u == valid.count(chain)) { qInfo() << "Invalid chain"; }

    try {
        const auto type = static_cast<ot::blockchain::Type>(chain);
        const auto handle = api_.Network().Blockchain().GetChain(type);

        if (false == handle.IsValid()) {
            throw std::runtime_error{"invalid chain"};
        }

        handle.get().Wallet().StartRescan();
    } catch (const std::exception& e) {
        qInfo() << e.what();
    }
}

auto Api::Imp::rpc(void* socket, zmq::Message&& in) const noexcept -> void
{
    const auto payload = in.Payload();

    if (payload.empty()) {
        qInfo() << "Invalid message";

        return;
    }

    const auto& cmd = payload[0];
    auto out = zmq::reply_to_message(std::move(in));

    if (ot_.RPC(cmd.Bytes(), out.AppendBytes())) {
        auto sent{true};
        const auto parts = out.size();
        auto counter = std::size_t{0};

        for (auto& frame : out.get()) {
            auto flags{0};

            if (++counter < parts) { flags |= ZMQ_SNDMORE; }

            sent &=
                (-1 != ::zmq_msg_send(
                           const_cast<ot::network::zeromq::Frame&>(frame),
                           socket,
                           flags));
        }

        if (false == sent) {
            qInfo() << "Send error: \n" << ::zmq_strerror(::zmq_errno());
        }
    }
}

auto Api::Imp::run_zmq() noexcept -> void
{
    {
        const auto endpoint = cli::rpc_endpoint();

        if (0 == ::zmq_bind(zmq_socket_, endpoint.c_str())) {
            qInfo()
                << QString("RPC socket opened at: %1").arg(endpoint.c_str());
        } else {
            qFatal("Failed to start RPC socket");
        }
    }

    auto poll = [&] {
        auto out = ot::Vector<::zmq_pollitem_t>{};
        auto& item = out.emplace_back();
        item.socket = zmq_socket_;
        item.events = ZMQ_POLLIN;

        return out;
    }();

    while (run_zmq_) {
        static constexpr auto timeout = 100ms;
        const auto events = ::zmq_poll(
            poll.data(), static_cast<int>(poll.size()), timeout.count());

        if (0 > events) {
            qInfo() << ::zmq_strerror(::zmq_errno());

            continue;
        } else if (0 == events) {

            continue;
        } else {
            for (auto& item : poll) {
                if (ZMQ_POLLIN != item.revents) { continue; }

                const auto& socket = item.socket;
                auto message = ot::network::zeromq::Message{};

                if (receive_message(socket, message)) {
                    rpc(socket, std::move(message));
                }
            }
        }
    }

    ::zmq_close(zmq_socket_);
}

auto Api::Imp::seedLanguageModel(const int type) -> model::SeedLanguage*
{
    ready().get();

    try {

        return seed_language_.at(static_cast<ot::crypto::SeedStyle>(type))
            .get();
    } catch (...) {

        return nullptr;
    }
}

auto Api::Imp::seedManager() const noexcept -> ot::ui::SeedTreeQt*
{
    static auto* model = api_.UI().SeedTreeQt();

    return model;
}

auto Api::Imp::seedSizeModel(const int type) -> model::SeedSize*
{
    ready().get();

    try {

        return seed_size_.at(static_cast<ot::crypto::SeedStyle>(type)).get();
    } catch (...) {

        return nullptr;
    }
}

auto Api::Imp::seedWordValidator(const int type, const int lang)
    -> const opentxs::ui::SeedValidator*
{
    ready().get();
    const auto style =
        static_cast<ot::crypto::SeedStyle>(static_cast<std::uint8_t>(type));
    const auto language =
        static_cast<ot::crypto::Language>(static_cast<std::uint8_t>(lang));

    return api_.UI().SeedValidator(style, language);
}

auto Api::Imp::validateBlockchains() const noexcept -> bool
{
    ready().get();
    auto lock = Lock{lock_};

    for (const auto chain : api_.Network().Blockchain().EnabledChains()) {
        if (false == make_accounts(chain)) { return false; }
    }

    return true;
}

auto Api::Imp::validateNym() const noexcept -> bool
{
    ready().get();
    auto lock = Lock{lock_};
    auto nymID = ot::identifier::Nym{};
    auto postcondition = ScopeGuard{[&] {
        if (nymID.empty()) { return; }

        {
            identityManager()->setActiveNym(
                QString::fromStdString(nymID.asBase58(api_.Crypto())));
            have_nym_ = true;
        }
    }};

    if (have_nym_) { return true; }

    const auto& config = api_.Config();
    const auto section = QGuiApplication::applicationName().toStdString();
    auto id = ot::UnallocatedCString{};
    auto rc = config.ReadString(section, nym_id_key, ot::writer(id));

    if (rc && (false == id.empty())) {
        nymID = api_.Factory().NymIDFromBase58(id);

        return true;
    }

    const auto nymList = api_.Wallet().LocalNyms();

    if (1 == nymList.size()) {
        const auto& firstID = *nymList.begin();
        id = firstID.asBase58(api_.Crypto());
        rc = config.WriteString(section, nym_id_key, id);

        if (false == rc) { qFatal("Failed to update configuration"); }

        if (false == config.Save()) { qFatal("Failed to save config file"); }

        nymID = firstID;

        return true;
    }

    return false;
}

auto Api::Imp::validateSeed() const noexcept -> bool
{
    ready().get();
    auto lock = Lock{lock_};

    if (false == seed_id_.empty()) { return true; }

    auto& seed = const_cast<opentxs::crypto::SeedID&>(seed_id_);
    const auto& config = api_.Config();
    const auto section = QGuiApplication::applicationName().toStdString();
    auto id = ot::UnallocatedCString{};
    auto rc = config.ReadString(section, seed_id_key, ot::writer(id));

    if (rc && (false == id.empty())) {
        seed = api_.Factory().SeedIDFromBase58(id);

        return true;
    }

    const auto seedList = api_.Storage().SeedList();

    if (1 == seedList.size()) {
        const auto& firstID = seedList.begin()->first;
        id = firstID;
        rc = config.WriteString(section, seed_id_key, id);

        if (false == rc) { qFatal("Failed to update configuration"); }

        if (false == config.Save()) { qFatal("Failed to save config file"); }

        seed = api_.Factory().SeedIDFromBase58(id);
        ;

        return true;
    }

    return false;
}

auto Api::Imp::wordCount(const int type, const int strength) -> int
{
    ready().get();
    const auto output = api_.Crypto().Seed().WordCount(
        static_cast<ot::crypto::SeedStyle>(static_cast<std::uint8_t>(type)),
        static_cast<ot::crypto::SeedStrength>(
            static_cast<std::size_t>(strength)));

    return static_cast<int>(output);
}

Api::Imp::~Imp()
{
    run_zmq_ = false;

    if (zmq_thread_.joinable()) { zmq_thread_.join(); }
}
}  // namespace metier::common
