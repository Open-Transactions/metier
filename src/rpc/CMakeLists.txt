# Copyright (c) 2019-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

target_sources(metier-rpc PRIVATE "rpc.cpp" "rpc.hpp")
target_link_libraries(metier-rpc PUBLIC Qt::Core)
target_compile_definitions(metier-rpc PRIVATE METIER_APP_NAME="${PROJECT_NAME}")
