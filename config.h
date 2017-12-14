/*****************************************************************************
* tunnelmanager - Simple GUI for SSH Tunnels
*
* Copyright (C) 2017 Syping
* Copyright (C) 2017 Soner Sayakci
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*
*****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#ifndef TM_APPVENDOR
#define TM_APPVENDOR "Shyim"
#endif

#ifndef TM_APPVENDORLINK
#define TM_APPVENDORLINK "https://github.com/shyim/"
#endif

#ifndef TM_APPSTR
#define TM_APPSTR "SSH Tunnel Manager"
#endif

#ifndef TM_APPDES
#define TM_APPDES "INSERT YOUR APPLICATION DESCRIPTION HERE"
#endif

#ifndef TM_COPYRIGHT
#define TM_COPYRIGHT "2017"
#endif

#ifndef TM_APPVER
#define TM_APPVER "1.0.0"
#endif

#ifdef TM_BUILDTYPE_REL
#ifndef TM_BUILDTYPE
#define TM_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Release")
#endif
#endif

#ifdef TM_BUILDTYPE_RC
#ifndef TM_BUILDTYPE
#define TM_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Release Candidate")
#endif
#endif

#ifdef TM_BUILDTYPE_DAILY
#ifndef TM_BUILDTYPE
#define TM_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Daily Build")
#endif
#endif

#ifdef TM_BUILDTYPE_DEV
#ifndef TM_BUILDTYPE
#define TM_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Developer")
#endif
#endif

#ifdef TM_BUILDTYPE_BETA
#ifndef TM_BUILDTYPE
#define TM_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Beta")
#endif
#endif

#ifdef TM_BUILDTYPE_ALPHA
#ifndef TM_BUILDTYPE
#define TM_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Alpha")
#endif
#endif

#ifndef TM_BUILDTYPE
#define TM_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Custom")
#endif

#ifdef TM_QCONF
#ifndef TM_SHARE
#define TM_SHARE "RUNDIR:SEPARATOR:..SEPARATOR:share"
#endif
#ifndef TM_LANG
#define TM_LANG "QCONFLANG:"
#endif
#ifndef TM_PLUG
#define TM_PLUG "QCONFPLUG:"
#endif
#ifdef TM_QCONF_IN
#ifndef TM_INLANG
#define TM_INLANG ":/tr"
#endif
#endif
#endif

#ifndef TM_SHARE
#define TM_SHARE "RUNDIR:"
#endif

#ifndef TM_LANG
#define TM_LANG "SHAREDDIR:SEPARATOR:lang"
#endif

#ifndef TM_PLUG
#define TM_PLUG "RUNDIR:SEPARATOR:plugins"
#endif

#ifdef TM_WINRT
#undef TM_WIN
#endif

#ifndef TM_COMPILER
#ifdef __clang__
#ifndef Q_OS_MAC
#define TM_COMPILER QString("Clang %1.%2.%3").arg(QString::number(__clang_major__), QString::number(__clang_minor__), QString::number(__clang_patchlevel__))
#else
#define TM_COMPILER QString("Apple LLVM %1.%2.%3").arg(QString::number(__clang_major__), QString::number(__clang_minor__), QString::number(__clang_patchlevel__))
#endif
#elif defined(__GNUC__)
#define TM_COMPILER QString("GCC %1.%2.%3").arg(QString::number(__GNUC__), QString::number(__GNUC_MINOR__), QString::number(__GNUC_PATCHLEVEL__))
#elif defined(__GNUG__)
#define TM_COMPILER QString("GCC %1.%2.%3").arg(QString::number(__GNUG__), QString::number(__GNUC_MINOR__), QString::number(__GNUC_PATCHLEVEL__))
#elif defined(_MSC_VER)
#define TM_COMPILER QString("MSVC %1").arg(QString::number(_MSC_VER).insert(2, "."))
#else
#define TM_COMPILER QString("Unknown Compiler")
#endif
#endif

#ifndef TM_BUILDDATETIME
#define TM_BUILDDATETIME QString("%1, %2").arg(__DATE__, __TIME__)
#endif

#ifndef TM_BUILDSTRING
#define TM_BUILDSTRING QString("%1, %2").arg(QT_VERSION_STR, TM_COMPILER)
#endif

#endif // CONFIG_H
