/*****************************************************************************
* tunnelmanager - Simple GUI for SSH Tunnels
*
* Copyright (C) 2017-2024 Syping
* Copyright (C) 2017 Soner Sayakci
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*
*****************************************************************************/

#include "AppEnv.h"
#include "config.h"
#include <QStringBuilder>
#include <QLibraryInfo>
#include <QApplication>
#include <QFileInfo>
#include <QScreen>
#include <QDir>

AppEnv::AppEnv()
{
}

// Folder Stuff

QString AppEnv::getExLangFolder()
{
    return convertBuildedString(TM_LANG);
}

QString AppEnv::getInLangFolder()
{
#ifdef TM_QCONF
#ifdef TM_INLANG
    return convertBuildedString(TM_INLANG);
#else
    return convertBuildedString(TM_SHARE % QLatin1String("SEPARATOR:APPDIRNAME:SEPARATOR:translations"));
#endif
#else
#ifdef TM_INLANG
    return convertBuildedString(TM_INLANG);
#else
    return QLatin1String(":/tr");
#endif
#endif
}

// Convert Stuff

QString AppEnv::convertBuildedString(const QString &buildedStr)
{
    QString outputStr = buildedStr;
    outputStr.replace("APPNAME:", QString::fromUtf8(TM_APPSTR));
    outputStr.replace("APPDIRNAME:", QString::fromUtf8(TM_APPDIRSTR));
    outputStr.replace("SHAREDDIR:", QString::fromUtf8(TM_SHARE));
    outputStr.replace("RUNDIR:", QFileInfo(QApplication::applicationFilePath()).canonicalPath());
#if QT_VERSION >= 0x060000
    outputStr.replace("QCONFLANG:", QLibraryInfo::path(QLibraryInfo::TranslationsPath));
    outputStr.replace("QCONFPLUG:", QLibraryInfo::path(QLibraryInfo::PluginsPath));
#else
    outputStr.replace("QCONFLANG:", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    outputStr.replace("QCONFPLUG:", QLibraryInfo::location(QLibraryInfo::PluginsPath));
#endif
    outputStr.replace("SEPARATOR:", QDir::separator());
    return outputStr;
}
