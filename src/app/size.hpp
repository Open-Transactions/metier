// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <tuple>

namespace metier
{
auto qml_default_size() noexcept -> std::pair<int, int>;
auto qml_minimum_size() noexcept -> std::pair<int, int>;
}  // namespace metier
