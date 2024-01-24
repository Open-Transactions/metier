// Copyright (c) 2010-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/external/cmrc.hpp"  // IWYU pragma: associated

namespace metier
{
auto cmrc_file_view(const std::filesystem::path& filename) noexcept
    -> std::string_view
{
    static const auto fs = cmrc::metier::get_filesystem();
    const auto file = fs.open(filename.string());

    return {file.cbegin(), file.cend()};
}
}  // namespace metier
