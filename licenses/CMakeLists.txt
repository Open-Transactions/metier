# Copyright (c) 2019-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_library(
  metier-license OBJECT
  "boost.h"
  "libzmq.h"
  "license.cpp"
  "license.hpp"
  "lmdb.h"
  "metier.h"
  "openssl.h"
  "qt.h"
  "sodium.h"
)
target_include_directories(
  metier-license
  INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${opentxs_INCLUDE_DIRS}"
)
target_compile_definitions(
  metier-license PRIVATE -DMETIER_APPSTREAM_NAME="${METIER_APPSTREAM_NAME}"
)
target_link_libraries(
  metier-license
  PRIVATE
    opentxs
    protobuf::libprotobuf-lite
    Qt::Core
    Qt::Gui
)

if(NOT METIER_BUNDLED_OPENTXS)
  target_include_directories(
    metier-license SYSTEM PRIVATE "${opentxs_INCLUDE_DIRS}"
  )
endif()
