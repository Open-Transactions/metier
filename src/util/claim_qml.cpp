// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "claim.hpp"  // IWYU pragma: associated

#include <QObject>
#include <QQmlApplicationEngine>

namespace metier
{
auto Ownership::Claim(QObject* pointer) noexcept -> void
{
    QQmlEngine::setObjectOwnership(pointer, QQmlEngine::CppOwnership);
}
}  // namespace metier
