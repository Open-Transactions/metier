# Copyright (c) 2019-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.21.0)
cmake_policy(
  SET
  CMP0091
  NEW
)
set(CMAKE_OSX_DEPLOYMENT_TARGET 11)
project(metier)
set(METIER_APPSTREAM_ID "org.opentransactions.metier")
set(METIER_APPSTREAM_NAME "Métier")
set(METIER_APP_DOMAIN "opentransactions.org")
set(METIER_CLI_COMMAND "metierctl")
set(METIER_GUI_OUTPUT_NAME ${PROJECT_NAME})

set(METIER_VERSION_MAJOR 23)
set(METIER_VERSION_MINOR 2)
set(METIER_VERSION_PATCH 4)

execute_process(
  COMMAND
    echo
    release_version=${METIER_VERSION_MAJOR}.${METIER_VERSION_MINOR}.${METIER_VERSION_PATCH}
  OUTPUT_FILE ${PROJECT_BINARY_DIR}/release_version.txt
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
  "${${PROJECT_NAME}_SOURCE_DIR}/deps/opentxs-cmake"
)

# -----------------------------------------------------------------------------
# Setup

include(otcommon-cmake)
otcommon_set_build_type()
otcommon_require_out_of_source_build()
otcommon_update_submodules()

# -----------------------------------------------------------------------------
# Set option defaults

if(CMAKE_BUILD_TYPE
   STREQUAL
   "Debug"
   OR CMAKE_BUILD_TYPE
      STREQUAL
      "RelWithDebInfo"
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

include(metier-configure-target)
otcommon_set_project_language_standard(
  99
  20
  ${METIER_PEDANTIC_BUILD}
)

# -----------------------------------------------------------------------------
# Print system information and build options

include(metier-print-build-details)
metier_print_build_details()

# -----------------------------------------------------------------------------
# Dependencies

otcommon_find_system_libraries()

find_package(
  QT
  REQUIRED
  NAMES
  Qt6
  Qt5
)

if(NOT METIER_BUNDLED_OPENTXS)
  find_package(opentxs CONFIG REQUIRED)
endif()

find_package(
  Boost
  1.82.0
  REQUIRED
  COMPONENTS
    date_time
    json
    program_options
  CONFIG
)
find_package(
  Qt${QT_VERSION_MAJOR}
  REQUIRED
  COMPONENTS
  Network
  Widgets
  CONFIG
)

if(METIER_QML_INTERFACE)
  find_package(
    Qt${QT_VERSION_MAJOR}
    REQUIRED
    COMPONENTS
      Qml
      Quick
      QuickControls2
      QmlImportScanner
      Svg
    CONFIG
  )
endif()

if(OT_USE_VCPKG_TARGETS)
  set(METIER_ZMQ_PACKAGE "ZeroMQ")
  set(METIER_ZMQ_TARGET "libzmq-static")
else()
  set(METIER_ZMQ_PACKAGE "unofficial-zeromq")
  set(METIER_ZMQ_TARGET "libzmq")
endif()

find_package("${METIER_ZMQ_PACKAGE}" QUIET)

# -----------------------------------------------------------------------------
# Build source

add_library(metier-common OBJECT "")
metier_configure_cxx_target(metier-common)
add_subdirectory(deps)
add_subdirectory(licenses)
add_subdirectory(src)
add_subdirectory(package)
