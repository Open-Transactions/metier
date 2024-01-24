# Copyright (c) 2010-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

function(metier_link_external LIB)
  include(metier-link-external-target)
  metier_link_external_target(metier-common "${LIB}")
endfunction()
