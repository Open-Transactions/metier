// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "cli/parser.hpp"  // IWYU pragma: associated

#include <boost/container/flat_map.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>

namespace po = boost::program_options;

namespace metier::cli
{
constexpr auto account_id_{"account"};
constexpr auto amount_{"amount"};
constexpr auto cmd_list_accounts_{"list_accounts"};
constexpr auto cmd_list_nyms_{"list_nyms"};
constexpr auto cmd_send_payment_{"send_payment"};
constexpr auto cmd_show_account_{"get_transactions"};
constexpr auto command_{"command"};
constexpr auto from_{"from"};
constexpr auto help_{"help"};
constexpr auto to_{"to"};
const auto usage_ =
    std::string{"Allowed commands:\n    "} + cmd_list_accounts_ + "\n    " +
    cmd_list_nyms_ + "\n    " + cmd_show_account_ + " --" + account_id_ +
    "=<account to query>\n    " + cmd_send_payment_ + " --" + from_ + "=<id> " +
    " --" + to_ + "=<address> " + " --" + amount_ + "=<value>";
using Map = boost::container::flat_map<Command, std::string>;
using ReverseMap = boost::container::flat_map<std::string, Command>;
auto command_map() noexcept -> const Map&;
auto command_map() noexcept -> const Map&
{
    static const auto map = Map{
        {Command::list_accounts, cmd_list_accounts_},
        {Command::list_nyms, cmd_list_nyms_},
        {Command::send_payment, cmd_send_payment_},
        {Command::show_account, cmd_show_account_},
    };

    return map;
}

struct Parser::Imp {
    static auto desc() -> const po::options_description&;
    static auto pos() -> const po::positional_options_description&;
};

auto Parser::Imp::desc() -> const po::options_description&
{
    static auto desc = []() {
        auto out = po::options_description{256};
        out.add_options()(help_, "Display this message");
        out.add_options()(command_, po::value<std::string>(), usage_.c_str());
        out.add_options()(
            account_id_, po::value<std::string>(), "target account id");
        out.add_options()(
            amount_, po::value<std::int64_t>(), "value to send as an integer");
        out.add_options()(from_, po::value<std::string>(), "source account id");
        out.add_options()(
            to_, po::value<std::string>(), "recipient address or payment code");

        return out;
    }();
    return desc;
}
auto Parser::Imp::pos() -> const po::positional_options_description&
{
    static auto pos = []() {
        auto out = po::positional_options_description{};
        out.add(command_, 1);

        return out;
    }();
    return pos;
};

Parser::Parser() noexcept
    : imp_(std::make_unique<Imp>())
{
}

auto Parser::help() const noexcept -> std::string
{
    auto out = std::stringstream{};
    out << "Usage:\n";
    out << Imp::desc();

    return out.str();
}

auto Parser::parse(int argc, char* argv[]) const noexcept -> Options
{
    auto vars = po::variables_map{};
    auto out = Options{};

    try {
        po::store(
            po::command_line_parser(argc, argv)
                .options(Imp::desc())
                .positional(Imp::pos())
                .run(),
            vars);
        po::notify(vars);

        for (const auto& [name, value] : vars) {
            if (name == help_) {
                out.show_help_ = true;
            } else if (name == command_) {
                out.command_ = translate(value.as<std::string>());
            } else if (name == account_id_) {
                out.account_id_ = value.as<std::string>();
            } else if (name == amount_) {
                out.amount_ = value.as<std::int64_t>();
            } else if (name == from_) {
                out.from_ = value.as<std::string>();
            } else if (name == to_) {
                out.to_ = value.as<std::string>();
            }
        }
    } catch (po::error& e) {
        std::cerr << "Syntax error: " << e.what() << "\n\n";
        out.show_help_ = true;

        return out;
    }

    if ((out.command_ == Command::show_account) && out.account_id_.empty()) {
        std::cerr << "Required argument --" << account_id_
                  << " not provided\n\n";
        out.show_help_ = true;

        return out;
    }

    if (out.command_ == Command::error) { out.show_help_ = true; }

    return out;
}

auto translate(Command in) noexcept -> std::string
{
    try {

        return command_map().at(in);
    } catch (...) {

        return "unknown command";
    }
}

auto translate(const std::string& in) noexcept -> Command
{
    static const auto map = [] {
        auto out = ReverseMap{};

        for (const auto& [cmd, string] : command_map()) {
            out.emplace(string, cmd);
        }

        return out;
    }();

    try {

        return map.at(in);
    } catch (...) {

        return Command::error;
    }
}

Parser::~Parser() = default;
}  // namespace metier::cli
