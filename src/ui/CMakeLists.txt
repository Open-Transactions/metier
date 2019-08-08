# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

qt5_wrap_ui(
  METIER_UI
  "blockchainchooser.ui"
  "blockchainsend.ui"
  "firstrun.ui"
  "licenses.ui"
  "mainwindow.ui"
  "profilealias.ui"
  "qrtoolbutton.ui"
  "receivingaddress.ui"
)

add_custom_target(metier-ui-generated ALL SOURCES "${METIER_UI}")
add_library(metier-ui INTERFACE)
target_include_directories(metier-ui INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")
add_dependencies(metier-ui metier-ui-generated)
