cmake_minimum_required(VERSION 3.16)

project(tunnelmanager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS LinguistTools QUIET)

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

if(Qt6LinguistTools_FOUND)
    qt6_add_translation(TUNNELMANAGER_TRANSLATIONS
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
        lang/tr_qt6.qrc
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

if(Qt6LinguistTools_FOUND AND QCONF_BUILD)
    add_dependencies(tunnelmanager translations)
endif()

target_precompile_headers(tunnelmanager PRIVATE config.h)

target_compile_definitions(tunnelmanager PRIVATE ${TUNNELMANAGER_DEFINES})
target_link_libraries(tunnelmanager PRIVATE Qt6::Widgets ${TUNNELMANAGER_LIBS})

install(TARGETS tunnelmanager DESTINATION bin)
install(FILES de.shyim.tunnelmanager.desktop DESTINATION share/applications)
install(FILES server.png DESTINATION share/pixmaps RENAME de.shyim.tunnelmanager.png)
if(QCONF_BUILD)
    install(FILES ${TUNNELMANAGER_TRANSLATIONS} DESTINATION share/tunnelmanager/translations)
endif()
