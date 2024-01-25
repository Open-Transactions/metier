# Copyright (c) 2010-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

if(OT_USE_VCPKG_TARGETS)
  find_package(
    Protobuf
    CONFIG
    REQUIRED
  )
else()
  find_package(Protobuf REQUIRED)
endif()

set(OPENTXS_PROTO_PACKAGE "metier.common.otproto")
set(METIER_GENERATED_PROTOBUFS "")

foreach(PROTO IN LISTS OT_PROTOBUF_FILES)
  configure_file(
    "${OT_PROTOBUF_DIR}/${PROTO}.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROTO}"
    @ONLY
  )
  list(
    APPEND
    METIER_GENERATED_PROTOBUFS
    "${CMAKE_CURRENT_BINARY_DIR}/${PROTO}"
  )
endforeach()

protobuf_generate_cpp(
  METIER_PROTO_GENERATED_SOURCE
  METIER_PROTO_GENERATED_HEADER
  ${METIER_GENERATED_PROTOBUFS}
)

add_library(
  metier-otproto OBJECT ${METIER_PROTO_GENERATED_SOURCE}
                        ${METIER_PROTO_GENERATED_HEADER}
)
metier_configure_cxx_target(metier-otproto)
target_link_libraries(metier-otproto PUBLIC protobuf::libprotobuf-lite)
target_include_directories(metier-otproto PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

if(MSVC)
  target_compile_options(metier-otproto PRIVATE "/wd4244" "/wd4267")
else()
  target_compile_options(
    metier-otproto
    PRIVATE
      -Wno-c++20-compat
      -Wno-effc++
      -Wno-extra-semi
      -Wno-extra-semi-stmt
      -Wno-gnu-offsetof-extensions
      -Wno-missing-declarations
      -Wno-reserved-identifier
      -Wno-shorten-64-to-32
      -Wno-suggest-destructor-override
      -Wno-switch-default
      -Wno-switch-enum
      -Wno-undef
      -Wno-unsafe-buffer-usage
      -Wno-unused-macros
  )
endif()

if(WIN32)
  set_target_properties(metier-otproto PROPERTIES UNITY_BUILD OFF)
endif()

set_target_properties(
  metier-otproto
  PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
    C_INCLUDE_WHAT_YOU_USE ""
    CXX_INCLUDE_WHAT_YOU_USE ""
    C_CLANG_TIDY ""
    CXX_CLANG_TIDY ""
)

add_dependencies(metier-common metier-otproto)
target_sources(metier-common PRIVATE $<TARGET_OBJECTS:metier-otproto>)
target_link_libraries(metier-common PUBLIC metier-otproto)
