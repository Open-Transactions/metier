# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(cxx-sources
    "accountactivity.cpp"
    "accountlist.cpp"
    "licenses.cpp"
    "profile.cpp"
    "seedlang.cpp"
    "seedsize.cpp"
    "seedtype.cpp"
)
set(cxx-headers "")
set(moc-headers
    "accountactivity.hpp"
    "accountlist.hpp"
    "blockchainchooser.hpp"
    "licenses.hpp"
    "profile.hpp"
    "seedlang.hpp"
    "seedsize.hpp"
    "seedtype.hpp"
)
qt5_wrap_cpp(moc-sources "${moc-headers}")

add_library(
  metier-models OBJECT
  "${cxx-sources}"
  "${cxx-headers}"
  "${moc-sources}"
  "${moc-headers}"
)
target_link_libraries(
  metier-models
  PUBLIC
    Qt5::Core
    Qt5::Gui
    protobuf::libprotobuf-lite
  PRIVATE metier-license opentxs
)
