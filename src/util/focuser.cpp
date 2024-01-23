// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "focuser.hpp"  // IWYU pragma: associated

#include <QWidget>

namespace metier::util
{
Focuser::Focuser(QWidget* w)
    : widget(w)
{
}

void Focuser::show()
{
    widget->show();
    focus();
}

void Focuser::focus()
{
    widget->activateWindow();
    widget->raise();
}
}  // namespace metier::util
