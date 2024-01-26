# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.26.0)
cmake_policy(
  SET
  CMP0091
  NEW
)
cmake_policy(
  SET
  CMP0126
  NEW
)

list(
  APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake"
)
include(metier-set-osx-deployment-target)
metier_set_osx_deployment_target(14)

project(metier)

# -----------------------------------------------------------------------------
# Setup

find_package(
  otcommon
  2
  CONFIG
  REQUIRED
)
include(metier-cmake-helpers)
otcommon_set_version_from_git()
otcommon_set_build_type()
otcommon_require_out_of_source_build()
otcommon_update_submodules()

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

if(DEFINED VCPKG_TARGET_TRIPLET)
  set(METIER_USE_VCPKG_TARGETS_DEFAULT ON)

  if(VCPKG_LIBRARY_LINKAGE
     STREQUAL
     "static"
  )
    set(METIER_STATIC_DEPENDENCIES_DEFAULT ON)
  else()
    set(METIER_STATIC_DEPENDENCIES_DEFAULT OFF)
  endif()
else()
  set(METIER_USE_VCPKG_TARGETS_DEFAULT OFF)
  set(METIER_STATIC_DEPENDENCIES_DEFAULT OFF)
endif()

# -----------------------------------------------------------------------------
# Options

option(
  METIER_PEDANTIC_BUILD
  "Treat compiler warnings as errors."
  ${METIER_PEDANTIC_DEFAULT}
)
option(
  METIER_USE_VCPKG_TARGETS
  "Assume vcpkg style names for FindPackage and target_link_libraries invocations"
  ${METIER_USE_VCPKG_TARGETS_DEFAULT}
)
option(
  METIER_STATIC_DEPENDENCIES
  "Assume dependencies are static libraries for FindPackage and target_link_libraries invocations"
  ${METIER_STATIC_DEPENDENCIES_DEFAULT}
)
option(
  METIER_ENABLE_SKIN_PROVIDER_METIER
  "Build metier skins"
  ON
)
option(
  METIER_IWYU
  "Enable include-what-you-use static analysis"
  OFF
)

if(METIER_IWYU)
  otcommon_make_iwyu_target()
endif()

if(METIER_ENABLE_SKIN_PROVIDER_METIER)
  list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/metier"
  )
  include(metier-configure-skin-provider-metier)
endif()

# -----------------------------------------------------------------------------
# Set compiler options

otcommon_set_project_language_standard(
  99
  20
  ${METIER_PEDANTIC_BUILD}
)

# -----------------------------------------------------------------------------
# Print system information and build options

metier_print_build_details()

# -----------------------------------------------------------------------------
# Dependencies

otcommon_find_system_libraries()

find_package(
  opentxs
  CONFIG
  REQUIRED
)

if(METIER_STATIC_DEPENDENCIES)
  set(METIER_ZMQ_TARGET "libzmq-static")
else()
  set(METIER_ZMQ_TARGET "libzmq")
endif()

if(METIER_USE_VCPKG_TARGETS)
  find_package(
    ZeroMQ
    CONFIG
    REQUIRED
  )
else()
  find_package(unofficial-zeromq REQUIRED)
endif()

# -----------------------------------------------------------------------------
# Build source

add_library(metier-common OBJECT)
target_include_directories(
  metier-common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src"
                       "${CMAKE_CURRENT_BINARY_DIR}/src"
)
target_link_libraries(metier-common PUBLIC opentxs::libopentxs)
metier_configure_cxx_target(metier-common)
metier_generate_cmrc_target()

add_subdirectory("data")
add_subdirectory("external")
add_subdirectory("src")
