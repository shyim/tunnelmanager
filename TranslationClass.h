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

#ifndef TRANSLATIONCLASS_H
#define TRANSLATIONCLASS_H

#include <QApplication>
#include <QTranslator>
#include <QStringList>
#include <QString>
#include <QObject>
#include <QLocale>

class TranslationClass : public QObject
{
    Q_OBJECT
public:
    static TranslationClass* getInstance() { return &translationClassInstance; }
    static QString getCountryCode(QLocale::Country country);
    static QString getCountryCode(QLocale locale);
    void initUserLanguage();
    void loadTranslation(QApplication *app);
    void unloadTranslation(QApplication *app);
    static QStringList listTranslations(const QString &langPath);
    QString getCurrentLanguage();
    bool isLanguageLoaded();

private:
    static TranslationClass translationClassInstance;
    bool loadSystemTranslation_p(const QString &langPath, QTranslator *appTranslator);
    bool loadUserTranslation_p(const QString &langPath, QTranslator *appTranslator);
    bool loadQtTranslation_p(const QString &langPath, QTranslator *qtTranslator);
    bool isUserLanguageSystem_p();
    QTranslator exAppTranslator;
    QTranslator exQtTranslator;
    QTranslator inAppTranslator;
    QTranslator inQtTranslator;
    QString userAreaLanguage;
    QString currentLanguage;
    QString userLanguage;
    int currentLangIndex;
    bool isEnglishMode;
    bool isLangLoaded;
};

extern TranslationClass translationClass;

#define TCIS TranslationClass::getInstance()

#endif // TRANSLATIONCLASS_H
