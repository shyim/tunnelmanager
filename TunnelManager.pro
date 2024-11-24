#/*****************************************************************************
#* tunnelmanager - Simple GUI for SSH Tunnels
#*
#* Copyright (C) 2017-2024 Syping
#* Copyright (C) 2017 Soner Sayakci
#*
#* This software may be modified and distributed under the terms
#* of the MIT license.  See the LICENSE file for details.
#*
#*****************************************************************************/

lessThan(QT_MAJOR_VERSION, 6) {
    error("Qt6 is required to build tunnelmanager")
}

QT += core gui widgets

TARGET = tunnelmanager
TEMPLATE = app

win32: RC_FILE = app.rc
win32: TARGET = TunnelManager
win32: CONFIG -= embed_manifest_exe

PRECOMPILED_HEADERS += config.h

SOURCES += main.cpp \
    mainwindow.cpp \
    newentry.cpp \
    AppEnv.cpp \
    TranslationClass.cpp

HEADERS += config.h \
    mainwindow.h \
    newentry.h \
    AppEnv.h \
    TranslationClass.h \

FORMS += \
    mainwindow.ui \
    newentry.ui

RESOURCES += \
    res.qrc \
    lang/tm_tr.qrc \
    lang/tr_qt6.qrc

TRANSLATIONS += \
    lang/tm_de.ts

OTHER_FILES += \
    app.rc \
    lang\tm_de.ts \
    lang\tm_de.qm \
    server.png \
    server.ico \
    TunnelManager.exe.manifest
