# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(cxx-sources
    "app.cpp"
    "factory.cpp"
    "legacy.cpp"
)
set(cxx-headers "imp.hpp")

if(METIER_QML_INTERFACE)
  list(
    APPEND
    cxx-sources
    "qml.cpp"
  )
  set(moc-headers "qml.hpp")
  qt5_wrap_cpp(moc-sources "${moc-headers}")
else()
  set(moc-headers "")
  set(moc-sources "")
endif()

add_library(
  metier-app OBJECT
  "${cxx-sources}"
  "${cxx-headers}"
  "${moc-sources}"
  "${moc-headers}"
)
target_link_libraries(
  metier-app
  PRIVATE
    Boost::headers
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    opentxs
)
target_compile_definitions(metier-app PRIVATE METIER_APP_NAME="${PROJECT_NAME}")

if(NOT METIER_BUNDLED_OPENTXS)
  target_include_directories(
    metier-app SYSTEM PRIVATE "${opentxs_INCLUDE_DIRS}"
  )
endif()

if(METIER_QML_INTERFACE)
  target_link_libraries(metier-app PRIVATE Qt5::Qml Qt5::Quick)
  target_compile_definitions(metier-app PRIVATE METIER_QML_INTERFACE=1)
else()
  target_compile_definitions(metier-app PRIVATE METIER_QML_INTERFACE=0)
endif()
