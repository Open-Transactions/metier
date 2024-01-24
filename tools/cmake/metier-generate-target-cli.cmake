# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

function(
  metier_generate_target_cli
  target_name
  exe_name
)

  configure_file(
    "${${PROJECT_NAME}_SOURCE_DIR}/src/metier/common/cli/endpoint.cpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/skin/cli_endpoint.cpp"
    @ONLY
  )

  add_executable(
    "${target_name}"
    "${target_name}.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/skin/cli_endpoint.cpp"
    $<TARGET_OBJECTS:metier-cli-common>
  )
  metier_configure_cxx_target("${target_name}")
  target_link_libraries("${target_name}" PUBLIC metier-cli-common)
  set_target_properties("${target_name}" PROPERTIES OUTPUT_NAME "${exe_name}")
endfunction()
