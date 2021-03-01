// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "profile.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>

namespace metier::model
{
struct Profile::Imp {
    opentxs::ui::ProfileQt* source_;

    Imp(opentxs::ui::ProfileQt* source) noexcept
        : source_(source)
    {
    }
};

Profile::Profile(opentxs::ui::ProfileQt* source) noexcept
    : ot_super(nullptr)
    , imp_(std::make_unique<Imp>(source))

{
    setSourceModel(source);
    using OT = opentxs::ui::ProfileQt;
    connect(imp_->source_, &OT::updated, [&] { emit updated(); });
}

auto Profile::displayName() const noexcept -> QString
{
    return imp_->source_->displayName();
}

auto Profile::paymentCode() const noexcept -> QString
{
    return imp_->source_->paymentCode();
}

Profile::~Profile() = default;
}  // namespace metier::model
