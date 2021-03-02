// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opentxs/opentxs.hpp>
#include <QValidator>

namespace ot = opentxs;

namespace metier::validate
{
class SeedWord final : public QValidator
{
public:
    auto fixup(QString& input) const -> void final
    {
        auto clean = QString{};

        for (const auto c : input) {
            if (c.isLetter()) { clean += c; }
        }

        auto notUsed{0};
        validate(clean, notUsed);
        input = clean;
    }
    auto validate(QString& input, int& pos) const -> State final
    {
        if (0 == input.size()) { return State::Intermediate; }

        const auto string = input.toStdString();
        const auto matches = [&] {
            // NOTE this logic needs to be moved to opentxs
            auto out = ot_.Seeds().ValidateWord(type_, lang_, string);
            out.erase(
                std::remove_if(
                    out.begin(),
                    out.end(),
                    [&](const auto& match) {
                        return match.size() < string.size();
                    }),
                out.end());

            return out;
        }();

        if (1 == std::count(matches.begin(), matches.end(), string)) {

            return State::Acceptable;
        }

        if (0 == matches.size()) {

            return State::Invalid;
        } else if (1 == matches.size()) {
            input = matches.begin()->c_str();
            pos = input.size();

            return State::Acceptable;
        } else {

            return State::Intermediate;
        }
    }

    SeedWord(
        const ot::api::client::Manager& ot,
        const ot::crypto::SeedStyle type,
        const ot::crypto::Language lang) noexcept
        : QValidator()
        , ot_(ot)
        , type_(type)
        , lang_(lang)
    {
    }

    ~SeedWord() final = default;

private:
    const ot::api::client::Manager& ot_;
    const ot::crypto::SeedStyle type_;
    const ot::crypto::Language lang_;
};
}  // namespace metier::validate
