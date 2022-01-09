// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <utility>

class QWidget;

namespace metier::util
{
using Ratio = std::pair<int, int>;

auto line_height(QWidget& widget, const Ratio lineHeight = {1, 1}) noexcept
    -> int;
auto line_width(QWidget& widget, const int widthInCharacters) noexcept -> int;
auto set_minimum_size(
    QWidget& widget,
    const int widthInCharacters,
    const int heightInLines,
    const Ratio lineHeight = {1, 1}) noexcept -> void;
}  // namespace metier::util
