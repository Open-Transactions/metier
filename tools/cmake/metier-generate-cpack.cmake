# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

macro(metier_generate_cpack)
  if(APPLE)
    # To generate the .dmg file, execute the following command in the build
    # folder: cpack -G DragNDrop
    #
    # Execute the command after cmake --build, but before cmake --install.

    set(CPACK_DRAGNDROP_COMPONENT_INSTALL TRUE)
    set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)

    if(CMAKE_SYSTEM_PROCESSOR
       MATCHES
       x86_64
    )
      set(METIER_PLATFORM "x86_64")
    elseif(
      CMAKE_SYSTEM_PROCESSOR
      MATCHES
      arm
    )
      set(METIER_PLATFORM "arm64")
    else()
      set(METIER_PLATFORM ${CPACK_SYSTEM_NAME})
    endif()

    set(CPACK_PACKAGE_NAME "${METIER_BASE_NAME}")
    set(CPACK_PACKAGE_VERSION
        ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}
    )
    # This sets the folder name displayed when the .dmg file is opened.
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${METIER_PLATFORM}-${METIER_PACKAGE_NAME_QUALIFIER}"
    )
    # This sets the name of the .dmg file.
    set(CPACK_DMG_APP-BUNDLE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${METIER_PLATFORM}-${METIER_PACKAGE_NAME_QUALIFIER}"
    )
    set(CPACK_GENERATOR "DRAGNDROP")

    include(CPack)
  endif()
endmacro()
