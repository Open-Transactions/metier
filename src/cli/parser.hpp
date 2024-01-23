// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace metier::cli
{
enum class Command {
    error,
    list_accounts,
    list_nyms,
    send_payment,
    show_account,
};

struct Options {
    Command command_{Command::error};
    bool show_help_{false};
    std::string account_id_{};
    std::string from_{};
    std::string to_{};
    std::int64_t amount_{};
};

class Parser
{
public:
    auto help() const noexcept -> std::string;
    auto parse(int argc, char* argv[]) const noexcept -> Options;

    Parser() noexcept;
    ~Parser();

private:
    struct Imp;

    std::unique_ptr<Imp> imp_;
};

auto translate(Command) noexcept -> std::string;
auto translate(const std::string&) noexcept -> Command;
}  // namespace metier::cli
