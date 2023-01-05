// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "cli/processor.hpp"  // IWYU pragma: associated

#include <boost/container/flat_map.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/json.hpp>
#include <opentxs/opentxs.hpp>
#include <zmq.h>
#include <chrono>
#include <sstream>
#include <vector>

#include "cli/parser.hpp"
#include "rpc/rpc.hpp"

namespace ot = opentxs;
namespace json = boost::json;
namespace ptime = boost::posix_time;

namespace metier::cli
{
auto translate(Result code) noexcept -> std::string
{
    static const auto map = boost::container::flat_map<Result, std::string>{
        {Result::success, "success"},
        {Result::socket_not_ready, "rpc socket error"},
        {Result::send_error, "rpc send error"},
        {Result::receive_timeout, "rpc server timeout"},
        {Result::receive_error, "rpc receive error"},
    };

    try {

        return map.at(code);
    } catch (...) {

        return "unknown error";
    }
}

struct Processor::Imp {
    const std::string endpoint_;
    const int linger_;

    auto get_account_activity(const Options& data) noexcept -> std::string
    {
        const auto request =
            ot::rpc::request::GetAccountActivity{session_, {data.account_id_}};
        auto base = std::unique_ptr<ot::rpc::response::Base>{};
        auto out = json::object{};
        out["command"] = translate(data.command_);
        const auto result = send(request, base);

        if (Result::success == result) {
            const auto& reply = base->asGetAccountActivity();
            auto events = json::array{};

            if (0u < reply.Activity().size()) {
                out["account"] = reply.Activity().front().AccountID();
            }

            for (const auto& event : reply.Activity()) {
                auto tx = json::object{};
                tx["id"] = event.UUID();
                tx["time"] = [&] {
                    auto time = ot::Clock::to_time_t(event.Timestamp());
                    auto ptime = ptime::from_time_t(time);
                    auto facet = std::make_unique<ptime::time_facet>(
                        "%a %b %d %l:%M:%S %p %Y");
                    auto text = std::stringstream{};
                    text.imbue(std::locale(text.getloc(), facet.release()));
                    text << ptime;
                    auto str = text.str();

                    return str;
                }();
                tx["amount"] = event.ConfirmedAmount_str();
                events.emplace_back(std::move(tx));
            }

            out.emplace("transactions", std::move(events));
        } else {
            out["error"] = translate(result);
        }

        return serialize(out);
    }
    auto list_accounts(const Options& data) noexcept -> std::string
    {
        auto out = json::object{};
        auto balances = json::array{};
        out["command"] = translate(data.command_);
        auto error = std::string{};

        try {
            auto ids = [&] {
                auto val = json::array{};
                const auto request = ot::rpc::request::ListAccounts{session_};
                auto base = std::unique_ptr<ot::rpc::response::Base>{};
                const auto result = send(request, base);

                if (Result::success == result) {
                    const auto& reply = base->asListAccounts();

                    for (const auto& id : reply.AccountIDs()) {
                        val.emplace_back(id);
                    }
                } else {
                    error = translate(result);

                    throw std::runtime_error{""};
                }

                return val;
            }();

            auto accounts = [&] {
                auto arg = ot::rpc::request::Base::Identifiers{};
                std::transform(
                    ids.begin(),
                    ids.end(),
                    std::back_inserter(arg),
                    [](const auto& val) { return val.as_string().c_str(); });

                return arg;
            }();
            const auto request = ot::rpc::request::GetAccountBalance{
                session_, std::move(accounts)};
            auto base = std::unique_ptr<ot::rpc::response::Base>{};
            const auto result = send(request, base);

            if (Result::success == result) {
                const auto& reply = base->asGetAccountBalance();

                for (const auto& balance : reply.Balances()) {
                    auto account = json::object{};
                    account["id"] = balance.ID();
                    account["style"] = print(balance.Type());
                    account["name"] = balance.Name();
                    account["owner"] = balance.Owner();
                    account["issuer"] = balance.Issuer();
                    account["contract"] = balance.Unit();
                    account["confirmed"] = balance.ConfirmedBalance_str();
                    account["pending"] = balance.PendingBalance_str();
                    // TODO account type
                    balances.emplace_back(std::move(account));
                }
            } else {
                error = translate(result);

                throw std::runtime_error{""};
            }
        } catch (...) {
            out["error"] = std::move(error);
        }

        out.emplace("accounts", std::move(balances));

        return serialize(out);
    }
    auto list_nyms(const Options& data) noexcept -> std::string
    {
        const auto request = ot::rpc::request::ListNyms{session_};
        auto base = std::unique_ptr<ot::rpc::response::Base>{};
        auto out = json::object{};
        out["command"] = translate(data.command_);
        const auto result = send(request, base);

        if (Result::success == result) {
            const auto& reply = base->asListNyms();
            auto nyms = json::array{};

            for (const auto& id : reply.NymIDs()) { nyms.emplace_back(id); }

            out.emplace("nyms", std::move(nyms));
        } else {
            out["error"] = translate(result);
        }

        return serialize(out);
    }
    auto send_payment(const Options& data) noexcept -> std::string
    {
        const auto request = ot::rpc::request::SendPayment{
            session_, data.from_, data.to_, data.amount_};
        auto base = std::unique_ptr<ot::rpc::response::Base>{};
        auto out = json::object{};
        out["command"] = translate(data.command_);
        out["from"] = data.from_;
        out["to"] = data.to_;
        out["amount"] = data.amount_;
        const auto result = send(request, base);

        if (Result::success == result) {
            const auto& reply = base->asSendPayment();
            const auto& codes = reply.ResponseCodes();

            if (0 < codes.size()) {
                const auto& code = codes.front().second;
                using enum ot::rpc::ResponseCode;

                switch (code) {
                    case txid: {
                        out["txid"] = reply.Pending().front().second;
                    } break;
                    case invalid:
                    case success:
                    case bad_session:
                    case none:
                    case queued:
                    case unnecessary:
                    case retry:
                    case no_path_to_recipient:
                    case bad_server_argument:
                    case cheque_not_found:
                    case payment_not_found:
                    case start_task_failed:
                    case nym_not_found:
                    case add_claim_failed:
                    case add_contact_failed:
                    case register_account_failed:
                    case bad_server_response:
                    case workflow_not_found:
                    case unit_definition_not_found:
                    case session_not_found:
                    case create_nym_failed:
                    case create_unit_definition_failed:
                    case delete_claim_failed:
                    case account_not_found:
                    case move_funds_failed:
                    case register_nym_failed:
                    case contact_not_found:
                    case account_owner_not_found:
                    case send_payment_failed:
                    case transaction_failed:
                    case unimplemented:
                    case error:
                    default: {
                        out["error"] = print(code);
                    }
                }
            } else {
                out["error"] = "Invalid rpc response";
            }
        } else {
            out["error"] = translate(result);
        }

        return serialize(out);
    }

    Imp() noexcept
        : endpoint_(metier::rpc_endpoint())
        , linger_(0)
        , zmq_(::zmq_ctx_new(), &::zmq_ctx_shutdown)
        , socket_(::zmq_socket(zmq_.get(), ZMQ_REQ), &::zmq_close)
        , ready_(0 == ::zmq_connect(socket_.get(), endpoint_.c_str()))
    {
        ::zmq_setsockopt(socket_.get(), ZMQ_LINGER, &linger_, sizeof(linger_));
    }

    ~Imp()
    {
        if (socket_) { ::zmq_disconnect(socket_.get(), endpoint_.c_str()); }
    }

private:
    struct Frame {
        operator ::zmq_msg_t*() noexcept { return &zmq_; }

        Frame() noexcept
            : zmq_()
        {
        }
        Frame(const Frame& rhs) noexcept
            : zmq_()
        {
            const auto size = ::zmq_msg_size(&rhs.zmq_);
            ::zmq_msg_init_size(&zmq_, size);
            std::memcpy(
                ::zmq_msg_data(&zmq_),
                ::zmq_msg_data(const_cast<::zmq_msg_t*>(&rhs.zmq_)),
                size);
        }

        ~Frame() { ::zmq_msg_close(*this); }

    private:
        ::zmq_msg_t zmq_;
    };

    using Context = std::unique_ptr<void, decltype(&::zmq_ctx_shutdown)>;
    using Message = std::vector<Frame>;
    using Socket = std::unique_ptr<void, decltype(&::zmq_close)>;

    static constexpr auto session_{0};

    Context zmq_;
    Socket socket_;
    bool ready_;

    static auto add_frame(Message& message, std::size_t bytes = 0u) noexcept
        -> Frame&
    {
        auto& out = message.emplace_back();

        if (0u == bytes) {
            ::zmq_msg_init(out);
        } else {
            ::zmq_msg_init_size(out, bytes);
        }

        return out;
    }
    static auto pretty_print(
        const json::value& json,
        std::ostream& out,
        std::string* indent = nullptr) noexcept -> void
    {
        auto indent_ = std::string{};

        if (nullptr == indent) { indent = &indent_; }

        switch (json.kind()) {
            case json::kind::object: {
                out << "{\n";
                indent->append(4, ' ');
                const auto& obj = json.get_object();

                if (!obj.empty()) {
                    auto it = obj.begin();

                    for (;;) {
                        out << *indent << json::serialize(it->key()) << " : ";
                        pretty_print(it->value(), out, indent);

                        if (++it == obj.end()) { break; }

                        out << ",\n";
                    }
                }

                out << "\n";
                indent->resize(indent->size() - 4);
                out << *indent << "}";

            } break;
            case json::kind::array: {
                out << "[\n";
                indent->append(4, ' ');
                auto const& arr = json.get_array();

                if (!arr.empty()) {
                    auto it = arr.begin();

                    for (;;) {
                        out << *indent;
                        pretty_print(*it, out, indent);

                        if (++it == arr.end()) { break; }

                        out << ",\n";
                    }
                }

                out << "\n";
                indent->resize(indent->size() - 4);
                out << *indent << "]";
            } break;
            case json::kind::string: {
                out << json::serialize(json.get_string());

            } break;

            case json::kind::uint64: {
                out << json.get_uint64();
            } break;
            case json::kind::int64: {
                out << json.get_int64();
            } break;
            case json::kind::double_: {
                out << json.get_double();
            } break;
            case json::kind::bool_: {
                if (json.get_bool()) {
                    out << "true";
                } else {
                    out << "false";
                }
            } break;
            case json::kind::null: {
                out << "null";
            } break;
            default: {
            }
        }

        if (indent->empty()) { out << "\n"; }
    }

    auto serialize(const json::value& json) const noexcept -> std::string
    {
        auto out = std::stringstream{};
        pretty_print(json, out);

        return out.str();
    }
    auto wait() const noexcept -> bool
    {
        auto poll = std::array<zmq_pollitem_t, 1>{};
        poll[0].socket = socket_.get();
        poll[0].events = ZMQ_POLLIN;
        static constexpr auto timeout =
            std::chrono::milliseconds{std::chrono::seconds{10}};
        const auto events = ::zmq_poll(poll.data(), 1, timeout.count());

        if (0 >= events) { return false; }

        return true;
    }

    auto receive(Message& out) noexcept -> bool
    {
        auto receiving{true};

        while (receiving) {
            auto& frame = add_frame(out);
            const bool received =
                (-1 != ::zmq_msg_recv(frame, socket_.get(), ZMQ_DONTWAIT));

            if (false == received) { return false; }

            auto option = int{0};
            auto optionBytes{sizeof(option)};
            const bool haveOption =
                (-1 != ::zmq_getsockopt(
                           socket_.get(), ZMQ_RCVMORE, &option, &optionBytes));

            if (false == haveOption) { return false; }

            if (optionBytes != sizeof(option)) { return false; }

            if (1 != option) { receiving = false; }
        }

        return true;
    }
    template <typename Reply>
    auto send(const ot::rpc::request::Base& in, Reply& out) noexcept -> Result
    {
        if (false == ready_) { return Result::socket_not_ready; }

        auto request = Message{};
        auto reply = Message{};
        in.Serialize({[&](const auto bytes) -> ot::WriteBuffer {
            auto& frame = add_frame(request, bytes);
            auto* p = static_cast<std::byte*>(::zmq_msg_data(frame));

            return std::span<std::byte>{p, ::zmq_msg_size(frame)};
        }});

        if (false == send(request)) { return Result::send_error; }

        if (false == wait()) { return Result::receive_timeout; }

        if (false == receive(reply)) { return Result::receive_error; }

        if (0 == reply.size()) { return Result::receive_error; }

        auto& frame = reply.front();
        out = ot::rpc::response::Factory(
            {static_cast<char*>(::zmq_msg_data(frame)), ::zmq_msg_size(frame)});

        return Result::success;
    }
    auto send(Message& message) noexcept -> bool
    {
        const auto parts = message.size();
        auto sent{true};
        auto counter = std::size_t{0};

        for (auto& frame : message) {
            auto flags = int{0};

            if (++counter < parts) { flags |= ZMQ_SNDMORE; }

            sent |= (-1 != ::zmq_msg_send(frame, socket_.get(), flags));
        }

        return sent;
    }
};

Processor::Processor() noexcept
    : imp_(std::make_unique<Imp>())
{
}

auto Processor::process(const Options& data) noexcept -> std::string
{
    switch (data.command_) {
        case Command::list_accounts: {

            return imp_->list_accounts(data);
        }
        case Command::list_nyms: {

            return imp_->list_nyms(data);
        }
        case Command::send_payment: {

            return imp_->send_payment(data);
        }
        case Command::show_account: {

            return imp_->get_account_activity(data);
        }
        case Command::error:
        default: {

            return {};
        }
    }
}

auto Processor::value() const noexcept -> int { return 0; }

Processor::~Processor() = default;
}  // namespace metier::cli
