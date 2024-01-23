# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

target_sources(
  metier-common
  PRIVATE
    "focuser.cpp"
    "resizer.cpp"
    "claim.hpp"
    "convertblockchain.hpp"
    "focuser.hpp"
    "resizer.hpp"
    "scopeguard.hpp"
)

if(METIER_QML_INTERFACE)
  target_sources(metier-common PRIVATE "claim_qml.cpp")
else()
  target_sources(metier-common PRIVATE "claim_legacy.cpp")
endif()

target_link_libraries(metier-common PRIVATE Qt::Widgets)

if(METIER_QML_INTERFACE)
  target_link_libraries(metier-common PRIVATE Qt::Qml)
endif()
