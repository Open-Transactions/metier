// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/mainwindow.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <map>
#include <mutex>
#include <tuple>

namespace ot = opentxs;

namespace metier::widget
{
struct SyncProgress {
    using Progress = std::pair<int, int>;
    using Lock = std::unique_lock<std::mutex>;

    auto get(const ot::blockchain::Type chain) const noexcept -> Progress
    {
        auto lock = Lock{lock_};

        try {

            return progress_.at(chain);
        } catch (...) {

            return {};
        }
    }

    auto update(const ot::blockchain::Type chain, const Progress value) noexcept
    {
        try {
            auto lock = Lock{lock_};

            progress_[chain] = value;
        } catch ([[maybe_unused]] const std::system_error& e) {
            return;
        }
    }

    SyncProgress() noexcept
        : lock_()
        , progress_()
    {
    }

private:
    mutable std::mutex lock_;
    std::map<ot::blockchain::Type, Progress> progress_;
};
}  // namespace metier::widget
