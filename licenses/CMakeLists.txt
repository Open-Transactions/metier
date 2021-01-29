# Copyright (c) 2019-2020 The Open-Transactions developers
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
  "secp256k1.h"
  "sodium.h"
)
target_include_directories(
  metier-license
  INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(
  metier-license PRIVATE opentxs protobuf::libprotobuf-lite Qt5::Core
)
