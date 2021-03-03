// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "license.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <map>
#include <string>
#include <type_traits>

#include "boost.h"
#include "libzmq.h"
#include "lmdb.h"
#include "metier.h"
#include "openssl.h"
#include "qt.h"
#include "secp256k1.h"
#include "sodium.h"

auto width_ = std::size_t{0};

auto make_vector() noexcept -> LicenseData;
auto make_vector() noexcept -> LicenseData
{
    auto output = LicenseData{
        {METIER_APPSTREAM_NAME,
         std::string{reinterpret_cast<const char*>(LICENSE), LICENSE_len}},
        {"Boost",
         std::string{reinterpret_cast<const char*>(boost_txt), boost_txt_len}},
        {"libsecp256k1",
         std::string{
             reinterpret_cast<const char*>(secp256k1_txt), secp256k1_txt_len}},
        {"libsodium",
         std::string{
             reinterpret_cast<const char*>(sodium_txt), sodium_txt_len}},
        {"LMDB",
         std::string{reinterpret_cast<const char*>(lmdb_txt), lmdb_txt_len}},
        {"OpenSSL",
         std::string{
             reinterpret_cast<const char*>(openssl_txt), openssl_txt_len}},
        {"Qt", std::string{reinterpret_cast<const char*>(qt_txt), qt_txt_len}},
        {"ZeroMQ",
         std::string{
             reinterpret_cast<const char*>(libzmq_txt), libzmq_txt_len}},
    };
    const auto& ot = opentxs::LicenseData();
    std::copy(ot.begin(), ot.end(), std::back_inserter(output));
    std::sort(
        std::next(output.begin()),
        output.end(),
        [](const auto& lhs, const auto& rhs) {
            return std::lexicographical_compare(
                std::begin(lhs.first),
                std::end(lhs.first),
                std::begin(rhs.first),
                std::end(rhs.first),
                [](const auto& c1, const auto& c2) {
                    return std::tolower(c1) < std::tolower(c2);
                });
        });
    for (const auto& [key, value] : output) {
        width_ = std::max(width_, key.size());
    }

    return output;
}

auto LongestLicenseDescription() noexcept -> std::size_t { return width_; }

auto MetierLicenseData() noexcept -> const LicenseData&
{
    static const auto data{make_vector()};

    return data;
}
