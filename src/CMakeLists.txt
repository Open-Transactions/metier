# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(app)
add_subdirectory(models)
add_subdirectory(util)

if(NOT METIER_QML_INTERFACE)
  add_subdirectory(ui)
  add_subdirectory(widgets)
endif()

set(cxx-sources "main.cpp" "otwrap.cpp")
set(cxx-headers "otwrap/imp.hpp")
set(moc-headers "app.hpp" "otwrap.hpp")
qt5_wrap_cpp(moc-sources "${moc-headers}")
set(object-deps
    $<TARGET_OBJECTS:metier-app>
    $<TARGET_OBJECTS:metier-license>
    $<TARGET_OBJECTS:metier-models>
    $<TARGET_OBJECTS:metier-util>
    $<TARGET_OBJECTS:qr-code-generator>
)

if(METIER_QML_INTERFACE)
  qt5_add_resources(rcc-sources "skin/qml/resources.qrc")
else()
  qt5_add_resources(rcc-sources "skin/default/resources.qrc")
  list(
    APPEND
    object-deps
    $<TARGET_OBJECTS:metier-widgets>
  )
endif()

if(WIN32)
  set(APP_ICON_RESOURCE_WINDOWS
      "${${PROJECT_NAME}_SOURCE_DIR}/package/windows/app.rc"
  )
else()
  set(APP_ICON_RESOURCE_WINDOWS "")
endif()

add_executable(
  "${PROJECT_NAME}"
  "${object-deps}"
  "${cxx-sources}"
  "${cxx-headers}"
  "${moc-sources}"
  "${moc-headers}"
  "${rcc-sources}"
  "${APP_ICON_RESOURCE_WINDOWS}"
)

target_include_directories(
  "${PROJECT_NAME}" PRIVATE "${${PROJECT_NAME}_SOURCE_DIR}"
)

target_link_libraries(
  "${PROJECT_NAME}"
  PRIVATE
    opentxs
    qr-code-generator
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Threads::Threads
    ${Boost_LIBRARIES}
    Boost::program_options
)

if(METIER_QML_INTERFACE)
  target_link_libraries("${PROJECT_NAME}" PRIVATE Qt5::Qml Qt5::Quick)
else()
  target_link_libraries("${PROJECT_NAME}" PRIVATE metier-ui Qt5::Widgets)
endif()

target_compile_definitions(
  "${PROJECT_NAME}" PRIVATE METIER_APP_DOMAIN="${METIER_APP_DOMAIN}"
                            METIER_APP_NAME="${PROJECT_NAME}"
)

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

install(TARGETS "${PROJECT_NAME}")
