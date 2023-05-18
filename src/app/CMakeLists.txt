# Copyright (c) 2019-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

if(METIER_QML_INTERFACE)
  target_sources(
    metier-common
    PRIVATE
      "qml.cpp"
      "size.hpp"
      "startup.cpp"
      "startup.hpp"
  )
  target_link_libraries(metier-common PRIVATE Qt::Qml Qt::Quick)
else()
  target_sources(metier-common PRIVATE "no-qml.cpp")
endif()

target_sources(
  metier-common
  PRIVATE
    "app.cpp"
    "app.hpp"
    "factory.cpp"
    "imp.hpp"
    "legacy.cpp"
)

target_link_libraries(
  metier-common
  PUBLIC
    Boost::headers
    Qt::Core
    Qt::Gui
    Qt::Widgets
    opentxs::libopentxs
)
