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

#ifndef APPENV_H
#define APPENV_H

#include <QString>
#include <QUrl>

class AppEnv
{
public:
    AppEnv();

    // Folder Stuff
    static QString getExLangFolder();
    static QString getInLangFolder();
    static QString getPluginsFolder();

    // Screen Stuff
    static qreal screenRatio();
	
	// Convert Stuff
	static QString convertBuildedString(const QString &buildedStr);
};

#endif // APPENV_H
