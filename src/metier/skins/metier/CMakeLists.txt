# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network Widgets)

if(METIER_QML_INTERFACE)
  find_package(
    Qt${QT_VERSION_MAJOR} REQUIRED
    COMPONENTS
      Qml
      Quick
      QuickControls2
      QmlImportScanner
      Svg
  )
endif()

set(METIER_BASE_NAME "metier")
set(METIER_APPSTREAM_ID "org.opentransactions.metier")
set(METIER_APPSTREAM_NAME "Métier")
set(METIER_APP_DOMAIN "opentransactions.org")
set(METIER_CLI_OUTPUT_NAME "metierctl")
set(METIER_GUI_OUTPUT_NAME ${METIER_BASE_NAME})
set(METIER_COPYRIGHT_STRING
    "Copyright (c) 2019-2024 The Open-Transactions developers"
)
set(METIER_PACKAGE_NAME_QUALIFIER "")

metier_generate_target_gui(
  "${METIER_BASE_NAME}"
  "metier"
  "${METIER_GUI_OUTPUT_NAME}"
  "${${METIER_BASE_NAME}_SOURCE_DIR}/data/metier/skins/metier/package/windows/app.rc"
  "${${METIER_BASE_NAME}_SOURCE_DIR}/data/metier/skins/metier/package/icons"
  "app-1024.icns"
  "${METIER_COPYRIGHT_STRING}"
  "${METIER_APPSTREAM_ID}"
)
metier_generate_target_cli("metierctl" "${METIER_CLI_OUTPUT_NAME}")

add_subdirectory(util)
add_subdirectory(widget)

set_target_properties(
  metier PROPERTIES AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)

target_sources(metier PRIVATE "skin/app.imp.widgets.cpp")

if(METIER_QML_INTERFACE)
  metier_link_external_target(metier Qt::Qml)
  metier_link_external_target(metier Qt::Quick)
  metier_link_external_target(metier Qt::QuickControls2)
  metier_link_external_target(metier Qt::Svg)

  if("${QT_VERSION_MAJOR}"
     EQUAL
     "5"
  )
    metier_link_external_target(metier Qt5::QSvgPlugin)
    metier_link_external_target(metier Qt5::QSvgIconPlugin)
  endif()

  qt_import_qml_plugins("${METIER_BASE_NAME}")
  target_sources(metier PRIVATE "skin/app.imp.qml.cpp")
else()
  target_sources(metier PRIVATE "skin/app.imp.no-qml.cpp")
endif()

if(APPLE)
  install(TARGETS "metier" BUNDLE DESTINATION . COMPONENT APP-BUNDLE)
else()
  install(TARGETS "metier" COMPONENT APP)
endif()

install(TARGETS "metierctl" COMPONENT CLI)

metier_generate_cpack()
metier_package_metadata(
  "${METIER_APPSTREAM_ID}"
  "${METIER_APPSTREAM_NAME}"
  "${METIER_GUI_OUTPUT_NAME}"
  "metier"
  "metier"
)
metier_generate_macos_packaging_files(
  "${METIER_APPSTREAM_ID}" "${METIER_GUI_OUTPUT_NAME}"
)
