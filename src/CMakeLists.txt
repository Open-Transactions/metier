# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(metier-common PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

if(WIN32)
  set(APP_ICON_RESOURCE "${${PROJECT_NAME}_SOURCE_DIR}/package/windows/app.rc")
elseif(APPLE)
  set(APP_ICON_RESOURCE
      "${${PROJECT_NAME}_SOURCE_DIR}/package/icons/app-1024.icns"
  )
else()
  set(APP_ICON_RESOURCE "")
endif()

add_library(metier-rpc OBJECT "")
metier_configure_cxx_target(metier-rpc)

add_subdirectory(api)
add_subdirectory(app)
add_subdirectory(cli)
add_subdirectory(models)
add_subdirectory(rpc)
add_subdirectory(skin)
add_subdirectory(util)
add_subdirectory(widgets)

target_sources(metier-common PRIVATE "main.cpp")
set_target_properties(
  metier-common
  PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)
set(object-deps
    $<TARGET_OBJECTS:metier-common>
    $<TARGET_OBJECTS:metier-rpc>
    $<TARGET_OBJECTS:qr-code-generator>
)

add_executable(
  "${PROJECT_NAME}" MACOSX_BUNDLE "${object-deps}" "${APP_ICON_RESOURCE}"
)
metier_configure_cxx_target("${PROJECT_NAME}")

target_include_directories(
  "${PROJECT_NAME}" PRIVATE "${${PROJECT_NAME}_SOURCE_DIR}"
)

target_link_libraries(
  "${PROJECT_NAME}"
  PRIVATE
    ${Boost_LIBRARIES}
    Boost::program_options
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Widgets
    Threads::Threads
    metier-rpc
    opentxs::libopentxs
    qr-code-generator
)

if(METIER_QML_INTERFACE)
  target_link_libraries(
    "${PROJECT_NAME}"
    PRIVATE
      Qt::Qml
      Qt::Quick
      Qt::QuickControls2
      Qt::Svg
  )

  if("${QT_VERSION_MAJOR}"
     EQUAL
     "5"
  )
    target_link_libraries(
      "${PROJECT_NAME}" PRIVATE Qt5::QSvgPlugin Qt5::QSvgIconPlugin
    )
  endif()

  qt_import_qml_plugins("${PROJECT_NAME}")
endif()

if(APPLE)
  install(TARGETS "${PROJECT_NAME}" BUNDLE DESTINATION . COMPONENT APP-BUNDLE)
else()
  install(TARGETS "${PROJECT_NAME}" COMPONENT APP)
endif()

if(APPLE)
  # To generate the .dmg file, execute the following command in the build
  # folder: cpack -G DragNDrop
  #
  # Execute the command after cmake --build, but before cmake --install.

  set(CPACK_DRAGNDROP_COMPONENT_INSTALL TRUE)
  set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
  set(CPACK_DMG_APP-BUNDLE_FILE_NAME
      "${METIER_GUI_OUTPUT_NAME}-${METIER_VERSION_MAJOR}.${METIER_VERSION_MINOR}.${METIER_VERSION_PATCH}"
  )
  set(CPACK_DMG_CLI_FILE_NAME
      "${METIER_CLI_COMMAND}-${METIER_VERSION_MAJOR}.${METIER_VERSION_MINOR}.${METIER_VERSION_PATCH}"
  )

  get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
  list(
    REMOVE_ITEM
    CPACK_COMPONENTS_ALL
    secp256k1
  )

  set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
  set(CPACK_PACKAGE_VERSION_MAJOR ${METIER_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${METIER_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${METIER_VERSION_PATCH})

  set(RESOURCE_FILES "${APP_ICON_RESOURCE}")

  set_target_properties(
    "${PROJECT_NAME}"
    PROPERTIES
      OUTPUT_NAME "${METIER_GUI_OUTPUT_NAME}"
      MACOSX_BUNDLE TRUE
      MACOSX_BUNDLE_BUNDLE_NAME "${METIER_GUI_OUTPUT_NAME}"
      MACOSX_BUNDLE_BUNDLE_VERSION
      "${METIER_VERSION_MAJOR}.${METIER_VERSION_MINOR}.${METIER_VERSION_PATCH}"
      MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2021 Open Transactions"
      MACOSX_BUNDLE_SHORT_VERSION_STRING
      "${METIER_VERSION_MAJOR}.${METIER_VERSION_MINOR}.${METIER_VERSION_PATCH}"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${METIER_APPSTREAM_ID}"
      MACOSX_BUNDLE_ICON_FILE "app-1024.icns"
      RESOURCE "${RESOURCE_FILES}"
  )

  set(CPACK_GENERATOR "DRAGNDROP")
  include(CPack)
elseif(WIN32)
  set_target_properties(
    "${PROJECT_NAME}"
    PROPERTIES
      OUTPUT_NAME "${METIER_GUI_OUTPUT_NAME}"
      RESOURCE "${RESOURCE_FILES}"
      WIN32_EXECUTABLE TRUE
  )
else()
  set_target_properties(
    "${PROJECT_NAME}" PROPERTIES OUTPUT_NAME "${METIER_GUI_OUTPUT_NAME}"
  )
endif()
