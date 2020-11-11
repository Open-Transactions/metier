# Copyright (c) 2019-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(models)
add_subdirectory(ui)
add_subdirectory(util)
add_subdirectory(widgets)

set(cxx-sources "app.cpp" "main.cpp" "otwrap.cpp" "otwrap/metiercallback.cpp")
set(cxx-headers
    "app_imp.hpp"
    "otwrap/imp.hpp"
    "otwrap/metiercallback.hpp"
    "otwrap/validateseed.hpp"
    "util/scopeguard.hpp"
)
set(moc-headers "app.hpp" "otwrap.hpp")
qt5_wrap_cpp(moc-sources "${moc-headers}")
qt5_add_resources(rcc-sources "resources.qrc")
add_executable(
  metier
  $<TARGET_OBJECTS:metier-license>
  $<TARGET_OBJECTS:metier-models>
  $<TARGET_OBJECTS:metier-util>
  $<TARGET_OBJECTS:metier-widgets>
  $<TARGET_OBJECTS:qr-code-generator>
  "${cxx-sources}"
  "${cxx-headers}"
  "${moc-sources}"
  "${moc-headers}"
  "${rcc-sources}"
)

target_link_libraries(
  metier
  PRIVATE
    metier-ui
    opentxs
    qr-code-generator
    Qt5::Core
    Qt5::Network
    Qt5::Widgets
    Threads::Threads
    ${Boost_LIBRARIES}
)

if(METIER_BUNDLED_OPENTXS)
  opentxs_link_target("metier")
else()
  target_link_libraries(metier PRIVATE protobuf::libprotobuf-lite)

  if(CMAKE_DL_LIBS)
    target_link_libraries(metier PRIVATE ${CMAKE_DL_LIBS})
  endif()

  if(LIB_RT)
    target_link_libraries(metier PRIVATE ${LIB_RT})
  endif()
endif()

install(TARGETS metier)
