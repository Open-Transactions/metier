# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_library(
  qr-code-generator OBJECT "QR-Code-generator/cpp/QrCode.hpp"
                           "QR-Code-generator/cpp/QrCode.cpp"
)

if(NOT MSVC)
  target_compile_options(
    qr-code-generator PRIVATE "-Wno-ctor-dtor-privacy"
                              "-Wno-shadow-field-in-constructor"
  )

  if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    target_compile_options(qr-code-generator PRIVATE "-Wno-effc++")
  endif()
endif()

target_include_directories(
  qr-code-generator SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
)
