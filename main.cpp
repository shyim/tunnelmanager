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

#include "TranslationClass.h"
#include "mainwindow.h"
#include "config.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION < 0x060000
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
    QApplication a(argc, argv);
    a.setApplicationName(TM_APPSTR);
    a.setApplicationVersion(TM_APPVER);

#ifdef Q_OS_WIN
    a.setFont(QApplication::font("QMenu"));
#endif

    bool startHidden = false;
    const QStringList args = a.arguments();
    for (const QString &arg : args) {
        if (arg == "--hidden")
            startHidden = true;
    }

    TCIS->initUserLanguage();
    TCIS->loadTranslation(&a);

    MainWindow w;
    if (!startHidden)
        w.showNormal();

    return a.exec();
}
