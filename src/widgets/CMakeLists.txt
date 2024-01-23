# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

target_sources(
  metier-common
  PRIVATE
    "enterpassphrase/imp.cpp"
    "blockchainsend/imp.hpp"
    "enterpassphrase/imp.hpp"
    "mainwindow/chaintoolboxmanager.hpp"
    "mainwindow/imp.hpp"
    "mainwindow/syncprogress.hpp"
    "newseed/imp.hpp"
    "recoverwallet/imp.hpp"
    "showseed/imp.hpp"
    "accountstatus.cpp"
    "accountstatus.hpp"
    "addcontact.cpp"
    "addcontact.hpp"
    "blockchainchooser.cpp"
    "blockchainchooser.hpp"
    "blockchainsend.cpp"
    "blockchainsend.hpp"
    "enterpassphrase.cpp"
    "enterpassphrase.hpp"
    "firstrun.cpp"
    "firstrun.hpp"
    "licenses.cpp"
    "licenses.hpp"
    "mainwindow.cpp"
    "mainwindow.hpp"
    "newseed.cpp"
    "newseed.hpp"
    "profilealias.cpp"
    "profilealias.hpp"
    "qrtoolbutton.cpp"
    "qrtoolbutton.hpp"
    "receivingaddress.cpp"
    "receivingaddress.hpp"
    "recoverwallet.cpp"
    "recoverwallet.hpp"
    "seedmanager.cpp"
    "seedmanager.hpp"
    "showseed.cpp"
    "showseed.hpp"
)

target_link_libraries(
  metier-common
  PUBLIC
    Qt::Core
    Qt::Network
    Qt::Widgets
    opentxs::libopentxs
    qr-code-generator
)
