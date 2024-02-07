# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

function(
  metier_generate_target_gui
  base_name
  target_name
  exe_name
  windows_icon
  apple_icon_path
  apple_icon_name
  copyright_string
  appstring_id
)
  include(metier-generate-target-exe)
  metier_generate_target_exe(
    "${base_name}"
    "${target_name}"
    "${exe_name}"
    "${windows_icon}"
    "${apple_icon_path}"
    "${apple_icon_name}"
    "${copyright_string}"
    "${appstring_id}"
    metier-common
    $<TARGET_OBJECTS:metier-common>
  )
  configure_file(
    "${${PROJECT_NAME}_SOURCE_DIR}/src/metier/common/app.skin.cpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/skin/app.skin.cpp"
    @ONLY
  )
  target_sources(
    "${target_name}"
    PRIVATE
      "${CMAKE_CURRENT_BINARY_DIR}/skin/app.skin.cpp"
      "skin/custom.cpp"
      "skin/size.cpp"
  )

  if(EXISTS "skin/app.imp.choose_interface.cpp")
    target_sources("${target_name}" PRIVATE "skin/app.imp.choose_interface.cpp")
  endif()
endfunction()
