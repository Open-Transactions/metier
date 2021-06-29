# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_executable(
  "${METIER_CLI_COMMAND}"
  "${APP_ICON_RESOURCE_WINDOWS}"
  "main.cpp"
  "parser.cpp"
  "parser.hpp"
  "processor.cpp"
  "processor.hpp"
  $<TARGET_OBJECTS:metier-rpc>
)

target_link_libraries(
  "${METIER_CLI_COMMAND}"
  PRIVATE
    "${METIER_ZMQ_TARGET}"
    Boost::program_options
    Boost::date_time
    metier-rpc
    opentxs
)

if(NOT METIER_BUNDLED_OPENTXS)
  target_include_directories(
    "${METIER_CLI_COMMAND}" SYSTEM PRIVATE "${opentxs_INCLUDE_DIRS}"
  )
endif()

install(TARGETS "${METIER_CLI_COMMAND}" COMPONENT METIER-CTL)
