# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(cxx-sources
    "enterpassphrase/imp.cpp"
    "addcontact.cpp"
    "blockchainchooser.cpp"
    "blockchainsend.cpp"
    "enterpassphrase.cpp"
    "firstrun.cpp"
    "licenses.cpp"
    "mainwindow.cpp"
    "newseed.cpp"
    "profilealias.cpp"
    "qrtoolbutton.cpp"
    "receivingaddress.cpp"
    "recoverwallet.cpp"
    "showseed.cpp"
)
set(cxx-headers
    "blockchainsend/imp.hpp"
    "enterpassphrase/imp.hpp"
    "mainwindow/chaintoolboxmanager.hpp"
    "mainwindow/imp.hpp"
    "mainwindow/syncprogress.hpp"
    "newseed/imp.hpp"
    "recoverwallet/imp.hpp"
    "showseed/imp.hpp"
)
set(moc-headers
    "addcontact.hpp"
    "blockchainchooser.hpp"
    "blockchainsend.hpp"
    "enterpassphrase.hpp"
    "firstrun.hpp"
    "licenses.hpp"
    "mainwindow.hpp"
    "newseed.hpp"
    "profilealias.hpp"
    "qrtoolbutton.hpp"
    "receivingaddress.hpp"
    "recoverwallet.hpp"
    "showseed.hpp"
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
  PUBLIC
    Qt5::Core
    Qt5::Network
    Qt5::Widgets
  PRIVATE
    metier-ui
    qr-code-generator
    opentxs
    protobuf::libprotobuf-lite
)
target_compile_definitions(
  metier-widgets PRIVATE -DMETIER_APPSTREAM_NAME="${METIER_APPSTREAM_NAME}"
)

if(NOT METIER_BUNDLED_OPENTXS)
  target_include_directories(
    metier-widgets SYSTEM PRIVATE "${opentxs_INCLUDE_DIRS}"
  )
endif()
