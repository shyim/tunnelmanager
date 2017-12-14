/*****************************************************************************
* tunnelmanager - Simple GUI for SSH Tunnels
*
* Copyright (C) 2017 Soner Sayakci
* Copyright (C) 2017 Syping
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*
*****************************************************************************/

#include "mainwindow.h"
#include "TranslationClass.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TCIS->initUserLanguage();
    TCIS->loadTranslation(&a);

    MainWindow w;
    w.show();

    return a.exec();
}
