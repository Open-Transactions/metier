# Copyright (c) 2010-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

function(
  metier_generate_macos_packaging_files
  appstream_id
  gui_output_name
)
  set(METIER_VERSION_STRING "${${PROJECT_NAME}_GIT_VERSION}")

  configure_file(
    "${${PROJECT_NAME}_SOURCE_DIR}/data/release_version.txt.in"
    "${CMAKE_CURRENT_BINARY_DIR}/release_version.txt"
    @ONLY
  )
  configure_file(
    "${${PROJECT_NAME}_SOURCE_DIR}/data/app_name.txt.in"
    "${CMAKE_CURRENT_BINARY_DIR}/app_name.txt"
    @ONLY
  )
  configure_file(
    "${${PROJECT_NAME}_SOURCE_DIR}/data/bundle_id.txt.in"
    "${CMAKE_CURRENT_BINARY_DIR}/bundle_id.txt"
    @ONLY
  )
endfunction()
