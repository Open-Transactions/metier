# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(cxx-sources
    "blockchainchooser.cpp"
    "blockchainsend.cpp"
    "firstrun.cpp"
    "licenses.cpp"
    "mainwindow.cpp"
    "profilealias.cpp"
    "qrtoolbutton.cpp"
    "receivingaddress.cpp"
)
set(cxx-headers
    "blockchainsend/imp.hpp"
    "mainwindow/chaintoolboxmanager.hpp"
    "mainwindow/imp.hpp"
    "mainwindow/syncprogress.hpp"
)
set(moc-headers
    "blockchainchooser.hpp"
    "blockchainsend.hpp"
    "firstrun.hpp"
    "licenses.hpp"
    "mainwindow.hpp"
    "profilealias.hpp"
    "qrtoolbutton.hpp"
    "receivingaddress.hpp"
)
qt5_wrap_cpp(moc-sources "${moc-headers}")

add_library(
  metier-widgets OBJECT
  "${cxx-sources}"
  "${cxx-headers}"
  "${moc-sources}"
  "${moc-headers}"
)
target_link_libraries(
  metier-widgets
  PUBLIC Qt5::Core Qt5::Network Qt5::Widgets
  PRIVATE metier-ui qr-code-generator opentxs
)
