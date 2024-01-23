// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "app/imp.hpp"

namespace metier
{
auto App::Imp::choose_interface(
    App& parent,
    int& argc,
    char** argv,
    bool) noexcept -> std::unique_ptr<Imp>
{
    return factory_widgets(parent, argc, argv);
}
}  // namespace metier
