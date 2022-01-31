# Copyright (c) 2019-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.15)
cmake_policy(
  SET
  CMP0091
  NEW
)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
project(metier)
set(METIER_APPSTREAM_ID "org.opentransactions.metier")
set(METIER_APPSTREAM_NAME "Métier")
set(METIER_APP_DOMAIN "opentransactions.org")
set(METIER_CLI_COMMAND "metierctl")
set(METIER_GUI_OUTPUT_NAME ${PROJECT_NAME})

set(METIER_VERSION_MAJOR 22)
set(METIER_VERSION_MINOR 2)
set(METIER_VERSION_PATCH 0)

execute_process(
  COMMAND
    echo
    release_version=${METIER_VERSION_MAJOR}.${METIER_VERSION_MINOR}.${METIER_VERSION_PATCH}
  OUTPUT_FILE release_version.txt
)
execute_process(
  COMMAND echo app_name=${METIER_GUI_OUTPUT_NAME}
  OUTPUT_FILE ${PROJECT_BINARY_DIR}/app_name.txt
)
execute_process(
  COMMAND echo cli_name=${METIER_CLI_COMMAND}
  OUTPUT_FILE ${PROJECT_BINARY_DIR}/cli_name.txt
)
execute_process(
  COMMAND echo bundle_id=${METIER_APPSTREAM_ID}
  OUTPUT_FILE ${PROJECT_BINARY_DIR}/bundle_id.txt
)

list(
  APPEND
  CMAKE_MODULE_PATH
  "${${PROJECT_NAME}_SOURCE_DIR}/cmake"
)
include(open-transactions-common)

# -----------------------------------------------------------------------------
# Setup

set_build_type()
set_out_of_source_build()
update_git_submodules()

# -----------------------------------------------------------------------------
# Set option defaults

if(CMAKE_BUILD_TYPE
   STREQUAL
   "Debug"
)
  set(METIER_PEDANTIC_DEFAULT ON)
  set(CMAKE_VERBOSE_MAKEFILE ON)
else()
  set(METIER_PEDANTIC_DEFAULT OFF)
endif()

if(WIN32)
  set(OT_USE_VCPKG_TARGETS_DEFAULT ON)
else()
  set(OT_USE_VCPKG_TARGETS_DEFAULT OFF)
endif()

# -----------------------------------------------------------------------------
# Options

option(
  METIER_PEDANTIC_BUILD
  "Treat compiler warnings as errors."
  ${METIER_PEDANTIC_DEFAULT}
)
option(
  METIER_BUNDLED_OPENTXS
  "Use submodule version of libopentxs"
  ON
)
option(
  OT_USE_VCPKG_TARGETS
  "Assume dependencies are managed by vcpkg"
  ${OT_USE_VCPKG_TARGETS_DEFAULT}
)
option(
  METIER_QML_INTERFACE
  "Use Qt Quick interface instead of Qt Widgets"
  OFF
)

# -----------------------------------------------------------------------------
# Set compiler options

set(METIER_ORIGINAL_C_FLAGS "${CMAKE_C_FLAGS}")
set(METIER_ORIGINAL_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set_compiler_flags(17 ${METIER_PEDANTIC_BUILD})

if(${CMAKE_CXX_COMPILER_ID}
   MATCHES
   Clang
)
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -Wno-redundant-parens -Wno-extra-semi-stmt -Wno-reserved-identifier"
  )
endif()

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-noexcept -Wno-c++20-compat")
endif()

# -----------------------------------------------------------------------------
# Print system information and build options

print_build_details(METIER_PEDANTIC_BUILD METIER_BUILD_TESTS)

if(METIER_QML_INTERFACE)
  message(STATUS "Interface type:           Qml")
else()
  message(STATUS "Interface type:           Widgets")
endif()

# -----------------------------------------------------------------------------
# Dependencies

find_system_libraries()

find_package(
  Boost
  1.75.0
  REQUIRED
  COMPONENTS
    date_time
    json
    program_options
)
find_package(
  QT
  NAMES
  Qt6
  Qt5
  COMPONENTS
    Core
    Gui
    Network
    Widgets
  REQUIRED
)

if(METIER_QML_INTERFACE)
  find_package(
    Qt${QT_VERSION_MAJOR}
    COMPONENTS
      Qml
      Quick
      QuickControls2
      QmlImportScanner
      Svg
    REQUIRED
  )
endif()

if(OT_USE_VCPKG_TARGETS)
  set(METIER_ZMQ_PACKAGE "ZeroMQ")
  set(METIER_ZMQ_TARGET "libzmq-static")
else()
  set(METIER_ZMQ_PACKAGE "unofficial-zeromq")
  set(METIER_ZMQ_TARGET "libzmq")
endif()

if(METIER_BUNDLED_OPENTXS)
  find_package(
    Boost
    1.75.0
    QUIET
    OPTIONAL_COMPONENTS
      container
      filesystem
      iostreams
      json
      stacktrace_basic
      system
      thread
  )

  find_package(ZLIB QUIET)
  find_package("${METIER_ZMQ_PACKAGE}" QUIET)
  find_package(
    OpenSSL
    1.0.1
    QUIET
  )
  find_package(unofficial-sodium QUIET)
  find_package(Protobuf QUIET)
  find_package(lmdb QUIET)
  find_package(
    Qt${QT_VERSION_MAJOR}
    COMPONENTS
      Core
      Gui
      Qml
      Widgets
    QUIET
  )
else()
  find_package(opentxs REQUIRED)
  find_package(
    Boost
    1.75.0
    QUIET
    OPTIONAL_COMPONENTS
      container
      filesystem
      iostreams
      json
      stacktrace_basic
      system
      thread
  )
  find_package(ZLIB QUIET)
  find_package("${METIER_ZMQ_PACKAGE}" QUIET)
  find_package(
    OpenSSL
    1.0.1
    QUIET
  )

  find_package(unofficial-secp256k1 QUIET)
  find_package(unofficial-sodium QUIET)
  find_package(Protobuf QUIET)
  find_package(lmdb QUIET)
  find_package(
    Qt${QT_VERSION_MAJOR} QUIET
    COMPONENTS
      Core
      Gui
      Qml
      Widgets
  )
  find_package(SQLite3 QUIET)
  find_package(opendht QUIET)
endif()

# -----------------------------------------------------------------------------
# Source Definitions

set_common_defines()
add_definitions("${QT_DEFINITIONS}")

# -----------------------------------------------------------------------------
# Build source

add_library(metier-common OBJECT "")
add_subdirectory(deps)
add_subdirectory(licenses)
add_subdirectory(src)
add_subdirectory(package)
