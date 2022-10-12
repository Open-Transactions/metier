# Copyright (c) 2019-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

configure_file(
  "version.cpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
  @ONLY
)

target_sources(
  metier-common
  PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
    "api.cpp"
    "api.hpp"
    "custom.cpp"
    "custom.hpp"
    "imp.hpp"
    "passwordcallback.cpp"
    "passwordcallback.hpp"
)
target_link_libraries(
  metier-common
  PUBLIC
    Qt::Core
    Qt::Gui
    opentxs
    qr-code-generator
    "${METIER_ZMQ_TARGET}"
)

if(NOT METIER_BUNDLED_OPENTXS)
  target_include_directories(
    metier-common SYSTEM PRIVATE "${opentxs_INCLUDE_DIRS}"
  )
endif()
