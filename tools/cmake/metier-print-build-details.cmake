# Copyright (c) 2010-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

macro(metier_print_build_details)
  include(otcommon-print-build-details)
  otcommon_print_build_details(METIER_PEDANTIC_BUILD METIER_BUILD_TESTS)

  if(METIER_QML_INTERFACE)
    message(STATUS "Interface type:           Qml")
  else()
    message(STATUS "Interface type:           Widgets")
  endif()
endmacro()
