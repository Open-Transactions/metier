# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

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

add_subdirectory(app)
add_subdirectory(cli)
add_subdirectory(models)
add_subdirectory(rpc)
add_subdirectory(ui)
add_subdirectory(util)
add_subdirectory(widgets)

set(cxx-sources
    "otwrap/imp.hpp"
    "otwrap/notary.cpp"
    "otwrap/notary.hpp"
    "otwrap/passwordcallback.cpp"
    "otwrap/passwordcallback.hpp"
    "main.cpp"
    "otwrap.cpp"
)
set(cxx-headers "otwrap/imp.hpp" "otwrap/passwordcallback.hpp")
set(moc-headers "app.hpp" "otwrap.hpp")
qt5_wrap_cpp(moc-sources "${moc-headers}")
set(object-deps
    $<TARGET_OBJECTS:metier-app>
    $<TARGET_OBJECTS:metier-license>
    $<TARGET_OBJECTS:metier-models>
    $<TARGET_OBJECTS:metier-rpc>
    $<TARGET_OBJECTS:metier-util>
    $<TARGET_OBJECTS:metier-widgets>
    $<TARGET_OBJECTS:qr-code-generator>
)

set(METIER_RCC_SOURCES "skin/default/qwidget.qrc")

if(METIER_QML_INTERFACE)
  list(
    APPEND
    METIER_RCC_SOURCES
    "skin/qml/resources.qrc"
  )
endif()

qt5_add_resources(rcc-sources "${METIER_RCC_SOURCES}")

add_executable(
  "${PROJECT_NAME}" MACOSX_BUNDLE
  "${object-deps}"
  "${cxx-sources}"
  "${cxx-headers}"
  "${moc-sources}"
  "${moc-headers}"
  "${rcc-sources}"
  "${APP_ICON_RESOURCE}"
)

target_include_directories(
  "${PROJECT_NAME}" PRIVATE "${${PROJECT_NAME}_SOURCE_DIR}"
)

if(NOT METIER_BUNDLED_OPENTXS)
  target_include_directories(
    "${PROJECT_NAME}" SYSTEM PRIVATE "${opentxs_INCLUDE_DIRS}"
  )
endif()

target_link_libraries(
  "${PROJECT_NAME}"
  PRIVATE
    ${Boost_LIBRARIES}
    Boost::program_options
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Widgets
    Threads::Threads
    metier-rpc
    opentxs
    qr-code-generator
)

if(METIER_QML_INTERFACE)
  target_link_libraries(
    "${PROJECT_NAME}"
    PRIVATE
      Qt5::Qml
      Qt5::QSvgPlugin
      Qt5::QSvgIconPlugin
      Qt5::Quick
      Qt5::Svg
  )
  qt5_import_qml_plugins("${PROJECT_NAME}")
endif()

target_compile_definitions(
  "${PROJECT_NAME}" PRIVATE METIER_APP_DOMAIN="${METIER_APP_DOMAIN}"
                            METIER_APP_NAME="${PROJECT_NAME}"
)
target_compile_definitions(metier-rpc PRIVATE METIER_APP_NAME="${PROJECT_NAME}")

if(METIER_USE_SYNC_SERVER)
  target_compile_definitions(
    "${PROJECT_NAME}"
    PRIVATE DEFAULT_SYNC_SERVER="${METIER_DEFAULT_SYNC_SERVER}"
  )
endif()

if(METIER_USE_DEFAULT_BLOCKCHAIN)
  target_compile_definitions(
    "${PROJECT_NAME}"
    PRIVATE METIER_DEFAULT_BLOCKCHAIN=${METIER_DEFAULT_BLOCKCHAIN}
  )
endif()

if(APPLE)
  install(
    TARGETS "${PROJECT_NAME}" BUNDLE DESTINATION . COMPONENT METIER-BUNDLE
  )
else()
  install(TARGETS "${PROJECT_NAME}" COMPONENT METIER_BIN)
endif()

if(APPLE)
  # To generate the .dmg file, execute the following command in the build
  # folder: cpack -G DragNDrop
  #
  # Execute the command after cmake --build, but before cmake --install.

  set(CPACK_DRAGNDROP_COMPONENT_INSTALL TRUE)
  set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
  set(CPACK_DMG_METIER-BUNDLE_FILE_NAME
      "metier-${METIER_VERSION_MAJOR}.${METIER_VERSION_MINOR}.${METIER_VERSION_PATCH}"
  )
  set(CPACK_DMG_METIER-CTL_FILE_NAME
      "metierctl-${METIER_VERSION_MAJOR}.${METIER_VERSION_MINOR}.${METIER_VERSION_PATCH}"
  )

  get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
  list(
    REMOVE_ITEM
    CPACK_COMPONENTS_ALL
    secp256k1
  )

  set(CPACK_PACKAGE_NAME metier)
  set(CPACK_PACKAGE_VERSION_MAJOR ${METIER_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${METIER_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${METIER_VERSION_PATCH})

  set(RESOURCE_FILES "${APP_ICON_RESOURCE}")

  set_target_properties(
    "${PROJECT_NAME}"
    PROPERTIES
      MACOSX_BUNDLE TRUE
      MACOSX_BUNDLE_BUNDLE_NAME "${METIER_APPSTREAM_NAME}"
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
    "${PROJECT_NAME}" PROPERTIES RESOURCE "${RESOURCE_FILES}" WIN32_EXECUTABLE
                                                              TRUE
  )
endif()
