# Copyright (c) 2010-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

function(
  metier_package_metadata
  METIER_APPSTREAM_ID
  METIER_APPSTREAM_NAME
  METIER_GUI_OUTPUT_NAME
  provider
  skin
)
  include(GNUInstallDirs)
  set(DATA_DIR
      "${${PROJECT_NAME}_SOURCE_DIR}/data/${provider}/skins/${skin}/package"
  )
  set(METIER_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
  set(METIER_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
  set(METIER_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")

  if(UNIX
     AND NOT APPLE
     AND NOT ANDROID
  )
    if(EXISTS "${DATA_DIR}/icons/app-1024.png")
      install(
        FILES "${DATA_DIR}/icons/app-1024.png"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/1024x1024/apps/"
        RENAME "${METIER_APPSTREAM_ID}.png"
      )
    endif()
    if(EXISTS "${DATA_DIR}/icons/app-512.png")
      install(
        FILES "${DATA_DIR}/icons/app-512.png"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/512x512/apps/"
        RENAME "${METIER_APPSTREAM_ID}.png"
      )
    endif()
    if(EXISTS "${DATA_DIR}/icons/app-128.png")
      install(
        FILES "${DATA_DIR}/icons/app-128.png"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/128x128/apps/"
        RENAME "${METIER_APPSTREAM_ID}.png"
      )
    endif()
    if(EXISTS "${DATA_DIR}/icons/app-64.png")
      install(
        FILES "${DATA_DIR}/icons/app-64.png"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/64x64/apps/"
        RENAME "${METIER_APPSTREAM_ID}.png"
      )
    endif()
    if(EXISTS "${DATA_DIR}/icons/app-48.png")
      install(
        FILES "${DATA_DIR}/icons/app-48.png"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/48x48/apps/"
        RENAME "${METIER_APPSTREAM_ID}.png"
      )
    endif()
    if(EXISTS "${DATA_DIR}/icons/app-32.png")
      install(
        FILES "${DATA_DIR}/icons/app-32.png"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/32x32/apps/"
        RENAME "${METIER_APPSTREAM_ID}.png"
      )
    endif()
    if(EXISTS "${DATA_DIR}/icons/app-16.png")
      install(
        FILES "${DATA_DIR}/icons/app-16.png"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/16x16/apps/"
        RENAME "${METIER_APPSTREAM_ID}.png"
      )
    endif()

    configure_file(
      "${DATA_DIR}/linux/desktop.in"
      "${CMAKE_CURRENT_BINARY_DIR}/${METIER_APPSTREAM_ID}.desktop"
      @ONLY
    )
    install(
      FILES "${CMAKE_CURRENT_BINARY_DIR}/${METIER_APPSTREAM_ID}.desktop"
      DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
    )

    configure_file(
      "${DATA_DIR}/linux/metainfo.xml.in"
      "${CMAKE_CURRENT_BINARY_DIR}/${METIER_APPSTREAM_ID}.metainfo.xml"
      @ONLY
    )
    install(
      FILES "${CMAKE_CURRENT_BINARY_DIR}/${METIER_APPSTREAM_ID}.metainfo.xml"
      DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/metainfo"
    )
  endif()
endfunction()
