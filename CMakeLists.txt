cmake_minimum_required(VERSION 3.5)

project(tunnelmanager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS LinguistTools QUIET)

if(WIN32)
    list(APPEND TUNNELMANAGER_DEFINES
        -DUNICODE
        -D_UNICODE
        -DWIN32
    )
    list(APPEND TUNNELMANAGER_RESOURCES
        app.rc
    )
endif()

set(TUNNELMANAGER_SOURCES
    main.cpp
    mainwindow.cpp
    newentry.cpp
    AppEnv.cpp
    TranslationClass.cpp
)

set(TUNNELMANAGER_HEADERS
    config.h
    mainwindow.h
    newentry.h
    AppEnv.h
    TranslationClass.h
)

set(TUNNELMANAGER_FORMS
    mainwindow.ui
    newentry.ui
)

list(APPEND TUNNELMANAGER_RESOURCES
    res.qrc
)

if(Qt5LinguistTools_FOUND)
    qt5_add_translation(TUNNELMANAGER_TRANSLATIONS
        lang/tm_de.ts
    )
    add_custom_target(translations DEPENDS ${TUNNELMANAGER_TRANSLATIONS})
else()
    set(TUNNELMANAGER_TRANSLATIONS
        lang/tm_de.qm
    )
endif()

option(QCONF_BUILD "System installation intended Qconf build" OFF)
if(QCONF_BUILD)
    list(APPEND TUNNELMANAGER_DEFINES
        -DTM_QCONF
    )
else()
    list(APPEND TUNNELMANAGER_RESOURCES
        lang/tm_tr.qrc
        lang/tr_qt5.qrc
    )
endif()

if(TUNNELMANAGER_APPVER)
    list(APPEND TUNNELMANAGER_DEFINES
        "-DTM_APPVER=\"${TUNNELMANAGER_APPVER}\""
    )
endif()
if(TUNNELMANAGER_BUILDTYPE)
    list(APPEND TUNNELMANAGER_DEFINES
        "-DTM_BUILDTYPE=\"${TUNNELMANAGER_BUILDTYPE}\""
    )
else()
    if(TUNNELMANAGER_BUILDTYPE_ALPHA)
        list(APPEND TUNNELMANAGER_DEFINES
            -DTM_BUILDTYPE_ALPHA
        )
    endif()
    if(TUNNELMANAGER_BUILDTYPE_BETA)
        list(APPEND TUNNELMANAGER_DEFINES
            -DTM_BUILDTYPE_BETA
        )
    endif()
    if(TUNNELMANAGER_BUILDTYPE_DEV)
        list(APPEND TUNNELMANAGER_DEFINES
            -DTM_BUILDTYPE_DEV
        )
    endif()
    if(TUNNELMANAGER_BUILDTYPE_DAILY)
        list(APPEND TUNNELMANAGER_DEFINES
            -DTM_BUILDTYPE_DAILY
        )
    endif()
    if(TUNNELMANAGER_BUILDTYPE_RC)
        list(APPEND TUNNELMANAGER_DEFINES
            -DTM_BUILDTYPE_RC
        )
    endif()
    if(TUNNELMANAGER_BUILDTYPE_REL)
        list(APPEND TUNNELMANAGER_DEFINES
            -DTM_BUILDTYPE_REL
        )
    endif()
endif()

if(TUNNELMANAGER_INLANG)
    list(APPEND TUNNELMANAGER_DEFINES
        "-DTM_INLANG=\"${TUNNELMANAGER_INLANG}\""
    )
endif()
if(TUNNELMANAGER_LANG)
    list(APPEND TUNNELMANAGER_DEFINES
        "-DTM_LANG=\"${TUNNELMANAGER_LANG}\""
    )
endif()

add_executable(tunnelmanager
    WIN32 MACOSX_BUNDLE
    ${TUNNELMANAGER_HEADERS}
    ${TUNNELMANAGER_SOURCES}
    ${TUNNELMANAGER_FORMS}
    ${TUNNELMANAGER_RESOURCES}
)

if(Qt5LinguistTools_FOUND AND QCONF_BUILD)
    add_dependencies(tunnelmanager translations)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.16.0")
    target_precompile_headers(tunnelmanager PRIVATE config.h)
endif()

target_compile_definitions(tunnelmanager PRIVATE ${TUNNELMANAGER_DEFINES})
target_link_libraries(tunnelmanager PRIVATE Qt5::Widgets ${TUNNELMANAGER_LIBS})

install(TARGETS tunnelmanager DESTINATION bin)
install(FILES de.shyim.tunnelmanager.desktop DESTINATION share/applications)
install(FILES server.png DESTINATION share/pixmaps RENAME de.shyim.tunnelmanager.png)
if(QCONF_BUILD)
    install(FILES ${TUNNELMANAGER_TRANSLATIONS} DESTINATION share/tunnelmanager/translations)
endif()
