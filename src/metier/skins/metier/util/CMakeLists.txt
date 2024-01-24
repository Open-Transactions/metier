# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

metier_add_private_header_target("metier" "focuser.hpp")
metier_add_private_header_target("metier" "resizer.hpp")
metier_add_source_file_target("metier" "focuser.cpp")
metier_add_source_file_target("metier" "resizer.cpp")
metier_link_external_target("metier" Qt::Widgets)
