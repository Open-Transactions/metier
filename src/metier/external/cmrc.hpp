// Copyright (c) 2010-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: no_include "cmrc/cmrc.hpp"

#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <cmrc/cmrc.hpp>  // IWYU pragma: export

#pragma GCC diagnostic pop

#include <filesystem>
#include <string_view>

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace cmrc
{
namespace detail
{
struct dummy;
}  // namespace detail
}  // namespace cmrc
// NOLINTEND(modernize-concat-nested-namespaces)

CMRC_DECLARE(metier);  // NOLINT

namespace metier
{
auto cmrc_file_view(const std::filesystem::path& filename) noexcept
    -> std::string_view;
}  // namespace metier
