// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "app.hpp"  // IWYU pragma: associated

#include <otwrap.hpp>

#include "widgets/blockchainchooser.hpp"
#include "widgets/firstrun.hpp"
#include "widgets/mainwindow.hpp"
#include "widgets/profilealias.hpp"

namespace metier
{
struct App::Imp {
    static std::unique_ptr<App> singleton_;

    std::atomic_bool first_run_complete_;
    std::unique_ptr<OTWrap> ot_;
    std::unique_ptr<widget::FirstRun> first_run_;
    std::unique_ptr<widget::ProfileAlias> profile_alias_;
    std::unique_ptr<widget::BlockchainChooser> blockchains_;
    std::unique_ptr<widget::MainWindow> main_window_;

    Imp(QApplication& parent) noexcept
        : first_run_complete_(false)
        , ot_(std::make_unique<OTWrap>(parent))
        , first_run_(std::make_unique<widget::FirstRun>(nullptr))
        , profile_alias_(std::make_unique<widget::ProfileAlias>(nullptr))
        , blockchains_(
              std::make_unique<widget::BlockchainChooser>(nullptr, *ot_))
        , main_window_(std::make_unique<widget::MainWindow>(nullptr, *ot_))
    {
    }
};
}  // namespace metier
