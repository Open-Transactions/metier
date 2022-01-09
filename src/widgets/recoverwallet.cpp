// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "widgets/recoverwallet.hpp"  // IWYU pragma: associated

#include "widgets/recoverwallet/imp.hpp"

namespace metier::widget
{
RecoverWallet::RecoverWallet(QObject* parent, OTWrap& ot) noexcept
    : QDialog(nullptr)
    , imp_p_(std::make_unique<Imp>(this, ot))
    , imp_(*imp_p_)
{
    moveToThread(parent->thread());
}

RecoverWallet::~RecoverWallet() = default;
}  // namespace metier::widget
