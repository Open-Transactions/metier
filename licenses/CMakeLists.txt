# Copyright (c) 2019-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

configure_file(
  "license.cpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/license.cpp"
  @ONLY
)

target_sources(
  metier-common
  PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/license.cpp"
    "boost.h"
    "libzmq.h"
    "license.hpp"
    "lmdb.h"
    "metier.h"
    "openssl.h"
    "qt.h"
    "sodium.h"
)

target_include_directories(
  metier-common PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${opentxs_INCLUDE_DIRS}"
)

target_include_directories(
  metier-common SYSTEM PRIVATE "${opentxs_INCLUDE_DIRS}"
)

target_link_libraries(
  metier-common
  PUBLIC
    opentxs
    Qt::Core
    Qt::Gui
)
