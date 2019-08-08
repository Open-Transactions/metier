# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

file(READ "${metier_SOURCE_DIR}/LICENSE" METIER_LICENSE_TEXT)
file(READ "boost.txt" BOOST_LICENSE_TEXT)
file(READ "libzmq.txt" ZMQ_LICENSE_TEXT)
file(READ "lmdb.txt" LMDB_LICENSE_TEXT)
file(READ "openssl.txt" OPENSSL_LICENSE_TEXT)
file(READ "qt.txt" QT_LICENSE_TEXT)
file(READ "secp256k1.txt" SECP256K1_LICENSE_TEXT)
file(READ "sodium.txt" SODIUM_LICENSE_TEXT)

configure_file("license.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/license.cpp" @ONLY)
add_library(
  metier-license OBJECT "license.hpp" "${CMAKE_CURRENT_BINARY_DIR}/license.cpp"
)
target_include_directories(
  metier-license
  INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(metier-license PRIVATE opentxs)
