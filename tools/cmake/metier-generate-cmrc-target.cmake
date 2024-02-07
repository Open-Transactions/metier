# Copyright (c) 2010-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

function(metier_generate_cmrc_target)
  find_package(
    CMakeRC
    CONFIG
    REQUIRED
  )

  set(ARG_TYPE "OBJECT")
  cmrc_add_resource_library(
    metier-cmrc
    NAMESPACE
    metier
    WHENCE
    "docs"
    "docs/license/3rd_party/boost"
    "docs/license/3rd_party/libzmq"
    "docs/license/3rd_party/lmdb"
    "docs/license/3rd_party/openssl"
    "docs/license/3rd_party/qrcode"
    "docs/license/3rd_party/qt"
    "docs/license/3rd_party/sodium"
  )
  cmrc_add_resources(
    metier-cmrc
    PREFIX
    "license/metier"
    "LICENSE"
  )
  otcommon_configure_target_external(metier-cmrc)
  target_link_libraries(metier-common PUBLIC metier-cmrc)
  target_sources(metier-common PUBLIC $<TARGET_OBJECTS:metier-cmrc>)
endfunction()
