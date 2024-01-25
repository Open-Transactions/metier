// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/skins/metier/widget/showseed.hpp"  // IWYU pragma: associated

#include "metier/skins/metier/widget/showseed/imp.hpp"

namespace metier::skins::metier::widget
{
ShowSeed::ShowSeed(QWidget* parent, common::Api& ot) noexcept
    : QDialog(parent)
    , imp_p_(std::make_unique<Imp>(this, ot))
    , imp_(*imp_p_)
{
}

ShowSeed::~ShowSeed() = default;
}  // namespace metier::skins::metier::widget
