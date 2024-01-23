// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/common/license.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>

#include "metier/external/cmrc.hpp"

namespace metier::common
{
using namespace std::literals;

auto LongestLicenseDescription(const LicenseData& data) noexcept -> std::size_t
{
    auto out = std::size_t{};
    const auto find_longest = [&out](const auto& in) {
        out = std::max(out, in.first.size());
    };
    std::ranges::for_each(data, find_longest);

    return out;
}

auto MetierLicenseData() noexcept -> const LicenseData&
{
    static const auto data = [] {
        auto out = LicenseData{};
        license_from_cmrc("Boost", "license/3rd_party/boost", out);
        license_from_cmrc("LMDB", "license/3rd_party/lmdb", out);
        license_from_cmrc("Métier", "license/metier/LICENSE", out);
        license_from_cmrc("OpenSSL", "license/3rd_party/openssl", out);
        license_from_cmrc(
            "QR Code generator library", "license/3rd_party/qrcode", out);
        license_from_cmrc("Qt", "license/3rd_party/qt", out);
        license_from_cmrc("ZeroMQ", "license/3rd_party/libzmq", out);
        license_from_cmrc("libsodium", "license/3rd_party/sodium", out);
        const auto& ot = opentxs::LicenseData();
        std::ranges::copy(ot, std::back_inserter(out));
        constexpr auto by_name = [](const auto& lhs, const auto& rhs) {
            return std::lexicographical_compare(
                std::begin(lhs.first),
                std::end(lhs.first),
                std::begin(rhs.first),
                std::end(rhs.first),
                [](const auto& c1, const auto& c2) {
                    return std::tolower(c1) < std::tolower(c2);
                });
        };
        std::ranges::sort(out, by_name);

        return out;
    }();

    return data;
}

auto license_from_cmrc(
    std::string_view dependency,
    const std::filesystem::path& filename,
    LicenseData& out) noexcept -> void
{
    out.emplace_back(std::make_pair(dependency, cmrc_file_view(filename)));
}
}  // namespace metier::common
