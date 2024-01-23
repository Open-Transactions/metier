# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

metier_add_private_header("licenses.hpp")
metier_add_private_header("seedlang.hpp")
metier_add_private_header("seedsize.hpp")
metier_add_private_header("seedtype.hpp")
metier_add_source_file("licenses.cpp")
metier_add_source_file("seedlang.cpp")
metier_add_source_file("seedsize.cpp")
metier_add_source_file("seedtype.cpp")
metier_link_external(Qt::Core)
metier_link_external(Qt::Gui)
