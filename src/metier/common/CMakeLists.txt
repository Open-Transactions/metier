# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_subdirectory(cli)
add_subdirectory(model)
add_subdirectory(otproto)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Qml)

set(METIER_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(METIER_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(METIER_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")

configure_file(
  "app.version.cpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/app.version.cpp"
  @ONLY
)

metier_add_private_header("api.hpp")
metier_add_private_header("api.imp.hpp")
metier_add_private_header("app.hpp")
metier_add_private_header("claim.hpp")
metier_add_private_header("convertblockchain.hpp")
metier_add_private_header("license.hpp")
metier_add_private_header("passwordcallback.hpp")
metier_add_private_header("scopeguard.hpp")
metier_add_private_header("size.hpp")
metier_add_private_header("startup.hpp")
metier_add_source_file("${CMAKE_CURRENT_BINARY_DIR}/app.version.cpp")
metier_add_source_file("api.cpp")
metier_add_source_file("api.imp.cpp")
metier_add_source_file("app.cpp")
metier_add_source_file("claim.cpp")
metier_add_source_file("convertblockchain.cpp")
metier_add_source_file("license.cpp")
metier_add_source_file("passwordcallback.cpp")
metier_add_source_file("scopeguard.cpp")
metier_add_source_file("startup.cpp")
metier_link_external("${METIER_ZMQ_TARGET}")
metier_link_external(Qt::Core)
metier_link_external(Qt::Gui)
metier_link_external(Qt::Qml)
metier_link_internal(Boost::headers)
