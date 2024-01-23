# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

function(
  metier_generate_target_cli
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
    metier-cli-common
    $<TARGET_OBJECTS:metier-cli-common>
  )
endfunction()
