/*****************************************************************************
* tunnelmanager - Simple GUI for SSH Tunnels
*
* Copyright (C) 2017-2020 Syping
* Copyright (C) 2017 Soner Sayakci
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*
*****************************************************************************/

#include "AppEnv.h"
#include "config.h"
#include <QStringBuilder>
#include <QDesktopWidget>
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
    return convertBuildedString(TM_SHARE % QLatin1String("SEPARATOR:APPNAME:SEPARATOR:translations"));
#endif
#else
#ifdef TM_INLANG
    return convertBuildedString(TM_INLANG);
#else
    return QString(":/tr");
#endif
#endif
}

QString AppEnv::getPluginsFolder()
{
    return convertBuildedString(TM_PLUG);
}

// Convert Stuff

QString AppEnv::convertBuildedString(const QString &buildedStr)
{
    QString outputStr = buildedStr;
    QByteArray sharePath = TM_SHARE;
    outputStr.replace("APPNAME:", TM_APPSTR);
    outputStr.replace("SHAREDDIR:", QString::fromUtf8(sharePath));
    outputStr.replace("RUNDIR:", QFileInfo(QApplication::applicationFilePath()).absoluteDir().absolutePath());
    outputStr.replace("QCONFLANG:", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    outputStr.replace("QCONFPLUG:", QLibraryInfo::location(QLibraryInfo::PluginsPath));
    outputStr.replace("SEPARATOR:", QDir::separator());
    return outputStr;
}
