// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

class QWidget;

namespace metier::util
{
class Focuser
{
public:
    Focuser(QWidget*);
    void show();
    void focus();

private:
    QWidget* widget;

    Focuser() = delete;
    Focuser(const Focuser&) = delete;
    Focuser(Focuser&&) = delete;
    Focuser& operator=(const Focuser&) = delete;
    Focuser& operator=(Focuser&&) = delete;
};
}  // namespace metier::util
