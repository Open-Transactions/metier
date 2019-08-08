# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.14)
project(metier)
list(APPEND CMAKE_MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/cmake")
include(open-transactions-common)

# -----------------------------------------------------------------------------
# Setup

set_build_type()
set_out_of_source_build()
update_git_submodules()

# -----------------------------------------------------------------------------
# Set option defaults

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(METIER_PEDANTIC_DEFAULT ON)
  set(CMAKE_VERBOSE_MAKEFILE ON)
else()
  set(METIER_PEDANTIC_DEFAULT OFF)
endif()

# -----------------------------------------------------------------------------
# Options

option(METIER_PEDANTIC_BUILD "Treat compiler warnings as errors."
       ${METIER_PEDANTIC_DEFAULT})

# -----------------------------------------------------------------------------
# Set compiler options

set_compiler_flags(17 ${METIER_PEDANTIC_BUILD})

if(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wno-redundant-parens -Wno-extra-semi-stmt"
  )
endif()


if(NOT MSVC)
  set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wno-noexcept"
  )
endif()

# -----------------------------------------------------------------------------
# Print system information and build options

print_build_details(METIER_PEDANTIC_BUILD METIER_BUILD_TESTS)

# -----------------------------------------------------------------------------
# Dependencies

find_system_libraries()

find_package(
  Boost 1.72.0 REQUIRED
  COMPONENTS
    system
    filesystem
    program_options
    thread
    iostreams
  OPTIONAL_COMPONENTS stacktrace_basic
)
find_package(OpenSSL 1.0.1 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Qt5 COMPONENTS Core Network Qml Widgets REQUIRED)
find_package(ZLIB REQUIRED)
find_package(opentxs REQUIRED)
find_package(lmdb REQUIRED)
find_package(unofficial-sodium REQUIRED)
find_package(unofficial-zeromq REQUIRED)
find_package(unofficial-secp256k1 REQUIRED)

# -----------------------------------------------------------------------------
# Source Definitions

set_common_defines()
add_definitions("${QT_DEFINITIONS}")

# -----------------------------------------------------------------------------
# Build source

add_subdirectory(deps)
add_subdirectory(licenses)
add_subdirectory(src)
