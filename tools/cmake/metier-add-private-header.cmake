# Copyright (c) 2010-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

macro(metier_add_private_header SOURCES)
  include(metier-add-private-header-target)
  metier_add_private_header_target(metier-common "${SOURCES}")
endmacro()
