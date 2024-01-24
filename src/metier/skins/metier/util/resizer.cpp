// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "metier/skins/metier/util/resizer.hpp"  // IWYU pragma: associated

#include <QFontMetrics>
#include <QWidget>

namespace metier::skins::metier::util
{
auto line_height(QWidget& widget, const Ratio lineHeight) noexcept -> int
{
    const auto font = widget.fontMetrics();
    const auto height = font.lineSpacing();

    return lineHeight.first * height / lineHeight.second;
}
auto line_width(QWidget& widget, const int widthInCharacters) noexcept -> int
{
    const auto font = widget.fontMetrics();

    return widthInCharacters * font.averageCharWidth();
}

auto set_minimum_size(
    QWidget& widget,
    const int widthInCharacters,
    const int heightInLines,
    const Ratio lineHeight) noexcept -> void
{
    const auto font = widget.fontMetrics();
    const auto width = font.averageCharWidth();
    const auto height = font.lineSpacing();
    widget.setMinimumSize(
        widthInCharacters * width,
        lineHeight.first * heightInLines * height / lineHeight.second);
}
}  // namespace metier::skins::metier::util
