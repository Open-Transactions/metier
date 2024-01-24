// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>
#include <string>

namespace metier
{
namespace common
{
namespace cli
{
struct Options;
}  // namespace cli
}  // namespace common
}  // namespace metier

namespace metier::common::cli
{
enum class Result {
    success,
    socket_not_ready,
    send_error,
    receive_timeout,
    receive_error,
};

auto translate(Result) noexcept -> std::string;

class Processor
{
public:
    auto value() const noexcept -> int;

    auto process(const Options& data) noexcept -> std::string;

    Processor() noexcept;
    ~Processor();

private:
    struct Imp;

    std::unique_ptr<Imp> imp_;
};
}  // namespace metier::common::cli
