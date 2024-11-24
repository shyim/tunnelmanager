/*****************************************************************************
* tunnelmanager - Simple GUI for SSH Tunnels
*
* Copyright (C) 2017-2021 Syping
* Copyright (C) 2017 Soner Sayakci
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*
*****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#if __cplusplus
#include <QString>
#define REL_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Release")
#define RC_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Release Candidate")
#define BETA_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Beta")
#define ALPHA_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Alpha")
#define DEV_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Developer")
#define DAILY_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Daily Build")
#define CUSTOM_BUILDTYPE QT_TRANSLATE_NOOP("ReleaseMark", "Custom")
#endif

#ifndef TM_APPVENDOR
#define TM_APPVENDOR "Shyim"
#endif

#ifndef TM_APPVENDORLINK
#define TM_APPVENDORLINK "https://github.com/shyim/"
#endif

#ifndef TM_APPSTR
#define TM_APPSTR "SSH Tunnel Manager"
#endif

#ifndef TM_APPDIRSTR
#define TM_APPDIRSTR "tunnelmanager"
#endif

#ifndef TM_APPDES
#define TM_APPDES "INSERT YOUR APPLICATION DESCRIPTION HERE"
#endif

#ifndef TM_COPYRIGHT
#define TM_COPYRIGHT "2017-2024"
#endif

#ifndef TM_APPVER
#define TM_APPVER "0.3.0"
#endif

#ifdef TM_QCONF
#ifndef TM_SHARE
#define TM_SHARE "RUNDIR:/../share"
#endif
#ifndef TM_LANG
#define TM_LANG "QCONFLANG:"
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
