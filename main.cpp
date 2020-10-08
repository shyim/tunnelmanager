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

#include "TranslationClass.h"
#include "mainwindow.h"
#include "config.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION >= 0x050600
#if QT_VERSION < 0x060000
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
#endif
    QApplication a(argc, argv);
    a.setApplicationName(TM_APPSTR);
    a.setApplicationVersion(TM_APPVER);

#ifdef Q_OS_WIN
#if QT_VERSION >= 0x050400
    a.setFont(QApplication::font("QMenu"));
#endif
#endif

    TCIS->initUserLanguage();
    TCIS->loadTranslation(&a);

    MainWindow w;
    w.show();
    w.adaptSize();

    return a.exec();
}
