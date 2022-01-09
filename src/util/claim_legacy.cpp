// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "claim.hpp"  // IWYU pragma: associated

#include <QObject>

namespace metier
{
auto Ownership::Claim(QObject*) noexcept -> void {}
}  // namespace metier
