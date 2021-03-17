# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(cxx-sources "")
set(cxx-headers
    "claim.hpp"
    "convertblockchain.hpp"
    "scopeguard.hpp"
)

if(METIER_QML_INTERFACE)
  list(
    APPEND
    cxx-sources
    "claim_qml.cpp"
  )
else()
  list(
    APPEND
    cxx-sources
    "claim_legacy.cpp"
    "focuser.cpp"
    "resizer.cpp"
  )
  list(
    APPEND
    cxx-headers
    "focuser.hpp"
    "resizer.hpp"
  )
endif()

add_library(metier-util OBJECT "${cxx-sources}" "${cxx-headers}")
target_link_libraries(metier-util PRIVATE Qt5::Widgets)

if(METIER_QML_INTERFACE)
  target_link_libraries(metier-util PRIVATE Qt5::Qml)
else()

endif()
