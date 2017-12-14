#/*****************************************************************************
#* tunnelmanager - Simple GUI for SSH Tunnels
#*
#* Copyright (C) 2017 Soner Sayakci
#* Copyright (C) 2017 Syping
#*
#* This software may be modified and distributed under the terms
#* of the MIT license.  See the LICENSE file for details.
#*
#*****************************************************************************/

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    lang\tm_tr.qrc \
    lang\tr_qt5.qrc

TRANSLATIONS += \
    lang\tm_de.ts

OTHER_FILES += \
    app.rc \
    lang\tm_de.ts \
    lang\tm_de.qm \
    server.png \
    server.ico \
    TranslationClass \
    TunnelManager.exe.manifest
