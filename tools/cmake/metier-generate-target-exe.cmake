# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

function(
  metier_generate_target_exe
  base_name
  target_name
  exe_name
  windows_icon
  apple_icon_path
  apple_icon_name
  copyright_string
  appstring_id
  base_target_name
  base_target_objects
)
  if(WIN32)
    set(APP_ICON_RESOURCE "${windows_icon}")
  elseif(APPLE)
    set(APP_ICON_RESOURCE "${apple_icon_path}/${apple_icon_name}")
  else()
    set(APP_ICON_RESOURCE "")
  endif()

  configure_file(
    "${${PROJECT_NAME}_SOURCE_DIR}/src/metier/common/cli/endpoint.cpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/skin/cli_endpoint.cpp"
    @ONLY
  )

  add_executable(
    "${target_name}" MACOSX_BUNDLE
    "${target_name}.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/skin/cli_endpoint.cpp"
    "${APP_ICON_RESOURCE}"
    "${base_target_objects}"
  )
  metier_configure_cxx_target("${target_name}")
  target_link_libraries("${target_name}" PUBLIC "${base_target_name}")
  set_target_properties(
    "${target_name}"
    PROPERTIES
      AUTOMOC ON
      AUTOUIC ON
      AUTORCC ON
      AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
  )

  if(APPLE)
    set_target_properties(
      "${target_name}"
      PROPERTIES
        OUTPUT_NAME "${exe_name}"
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "${base_name}"
        MACOSX_BUNDLE_COPYRIGHT "${copyright_string}"
        MACOSX_BUNDLE_BUNDLE_VERSION
        "${{PROJECT_NAME}_VERSION_MAJOR}.${{PROJECT_NAME}_VERSION_MINOR}.${{PROJECT_NAME}_VERSION_PATCH}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING
        "${{PROJECT_NAME}_VERSION_MAJOR}.${{PROJECT_NAME}_VERSION_MINOR}.${{PROJECT_NAME}_VERSION_PATCH}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${appstring_id}"
        MACOSX_BUNDLE_ICON_FILE "${apple_icon_name}"
        RESOURCE "${APP_ICON_RESOURCE}"
    )
  elseif(WIN32)
    set_target_properties(
      "${target_name}"
      PROPERTIES
        OUTPUT_NAME "${exe_name}"
        RESOURCE "${APP_ICON_RESOURCE}"
        WIN32_EXECUTABLE TRUE
    )
  else()
    set_target_properties("${target_name}" PROPERTIES OUTPUT_NAME "${exe_name}")
  endif()
endfunction()
