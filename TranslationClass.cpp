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
#include "AppEnv.h"
#include "config.h"
#include <QStringBuilder>
#include <QApplication>
#include <QStringList>
#include <QTranslator>
#include <QSettings>
#include <QLocale>
#include <QDebug>
#include <QFile>
#include <QDir>

#define QtBaseTranslationFormat "qtbase_"

TranslationClass TranslationClass::translationClassInstance;

void TranslationClass::initUserLanguage()
{
    QSettings settings(TM_APPVENDOR, TM_APPSTR);
    settings.beginGroup("Interface");
    userLanguage = settings.value("Language", "System").toString();
    settings.endGroup();
}

void TranslationClass::loadTranslation(QApplication *app)
{
    if (isLangLoaded) { unloadTranslation(app); }
    else { currentLangIndex = 0; }
    QString exLangPath = AppEnv::getExLangFolder();
    QString inLangPath = AppEnv::getInLangFolder();
    if (userLanguage == "en" || userLanguage == "en_GB")
    {
        currentLanguage = "en_GB";
        if (loadQtTranslation_p(exLangPath, &exQtTranslator))
        {
            app->installTranslator(&exQtTranslator);
        }
        else if (loadQtTranslation_p(inLangPath, &inQtTranslator))
        {
            app->installTranslator(&inQtTranslator);
        }
        QLocale::setDefault(QLocale(currentLanguage));
        isLangLoaded = true;
        return;
    }
#ifndef TM_QCONF // Classic modable loading method
    QString externalLanguageStr;
    bool externalLanguageReady = false;
    bool externalEnglishMode = false;
    bool loadInternalLang = false;
    bool trLoadSuccess = false;
    if (isUserLanguageSystem_p())
    {
#ifdef TM_DEBUG
        qDebug() << "loadExSystemLanguage";
#endif
        trLoadSuccess = loadSystemTranslation_p(exLangPath, &exAppTranslator);
    }
    else
    {
#ifdef TM_DEBUG
        qDebug() << "loadExUserLanguage";
#endif
        trLoadSuccess = loadUserTranslation_p(exLangPath, &exAppTranslator);
        if (!trLoadSuccess)
        {
#ifdef TM_DEBUG
            qDebug() << "loadInUserLanguage";
#endif
            trLoadSuccess = loadUserTranslation_p(inLangPath, &inAppTranslator);
            if (!trLoadSuccess)
            {
#ifdef TM_DEBUG
                qDebug() << "loadUserLanguageFailed";
#endif
            }
            else
            {
#ifdef TM_DEBUG
                qDebug() << "loadUserLanguageSuccess";
#endif
                loadInternalLang = true;
                isLangLoaded = true;
            }
        }
        else
        {
#ifdef TM_DEBUG
            qDebug() << "loadUserLanguageSuccess";
#endif
            isLangLoaded = true;
        }
    }
    if (trLoadSuccess)
    {
        if (currentLangIndex != 0 || isEnglishMode) // Don't install the language until we know we not have a better language for the user
        {
#ifdef TM_DEBUG
            qDebug() << "externalLanguageReady" << currentLanguage;
#endif
            externalEnglishMode = isEnglishMode;
            externalLanguageStr = currentLanguage;
            externalLanguageReady = true;
        }
        else
        {
#ifdef TM_DEBUG
            qDebug() << "installTranslation";
#endif
            if (loadInternalLang)
            {
                app->installTranslator(&inAppTranslator);
            }
            else
            {
                app->installTranslator(&exAppTranslator);
            }
            if (loadQtTranslation_p(exLangPath, &exQtTranslator))
            {
                app->installTranslator(&exQtTranslator);
            }
            else if (loadQtTranslation_p(inLangPath, &inQtTranslator))
            {
                app->installTranslator(&inQtTranslator);
            }
            QLocale::setDefault(QLocale(currentLanguage));
            isLangLoaded = true;
        }
    }
    if (externalLanguageReady)
    {
#ifdef TM_DEBUG
        qDebug() << "loadInSystemLanguage";
#endif
        int externalLangIndex = currentLangIndex;
        trLoadSuccess = loadSystemTranslation_p(inLangPath, &inAppTranslator);
#ifdef TM_DEBUG
        qDebug() << "externalLangIndex" << externalLangIndex << "internalLangIndex" << currentLangIndex;
        qDebug() << "externalEnglishMode" << externalEnglishMode << "internalEnglishMode" << isEnglishMode;
#endif
        if ((trLoadSuccess && externalLangIndex > currentLangIndex) || (trLoadSuccess && externalEnglishMode && !isEnglishMode))
        {
#ifdef TM_DEBUG
            qDebug() << "installInternalTranslation";
#endif
            app->installTranslator(&inAppTranslator);
            if (loadQtTranslation_p(exLangPath, &exQtTranslator))
            {
                app->installTranslator(&exQtTranslator);
            }
            else if (loadQtTranslation_p(inLangPath, &inQtTranslator))
            {
                app->installTranslator(&inQtTranslator);
            }
            QLocale::setDefault(QLocale(currentLanguage));
            isLangLoaded = true;
        }
        else
        {
#ifdef TM_DEBUG
            qDebug() << "installExternalTranslation";
#endif
            isEnglishMode = externalEnglishMode;
            currentLanguage = externalLanguageStr;
            app->installTranslator(&exAppTranslator);
            if (loadQtTranslation_p(exLangPath, &exQtTranslator))
            {
                app->installTranslator(&exQtTranslator);
            }
            else if (loadQtTranslation_p(inLangPath, &inQtTranslator))
            {
                app->installTranslator(&inQtTranslator);
            }
            QLocale::setDefault(QLocale(currentLanguage));
            isLangLoaded = true;
        }
    }
    else if (!isLangLoaded)
    {
#ifdef TM_DEBUG
        qDebug() << "loadInSystemLanguage";
#endif
        trLoadSuccess = loadSystemTranslation_p(inLangPath, &inAppTranslator);
        if (trLoadSuccess)
        {
#ifdef TM_DEBUG
            qDebug() << "installInternalTranslation";
#endif
            app->installTranslator(&inAppTranslator);
            if (loadQtTranslation_p(exLangPath, &exQtTranslator))
            {
                app->installTranslator(&exQtTranslator);
            }
            else if (loadQtTranslation_p(inLangPath, &inQtTranslator))
            {
                app->installTranslator(&inQtTranslator);
            }
            QLocale::setDefault(QLocale(currentLanguage));
            isLangLoaded = true;
        }
        else if (!trLoadSuccess)
        {
#ifdef TM_DEBUG
            qDebug() << "fallbackToDefaultApplicationLanguage";
#endif
            currentLanguage = "en_GB";
            if (loadQtTranslation_p(exLangPath, &exQtTranslator))
            {
                app->installTranslator(&exQtTranslator);
            }
            else if (loadQtTranslation_p(inLangPath, &inQtTranslator))
            {
                app->installTranslator(&inQtTranslator);
            }
            QLocale::setDefault(QLocale(currentLanguage));
            isLangLoaded = true;
        }
    }
#else // New qconf loading method
    bool trLoadSuccess;
    if (isUserLanguageSystem_p())
    {
        trLoadSuccess = loadSystemTranslation_p(inLangPath, &inAppTranslator);
    }
    else
    {
        trLoadSuccess = loadUserTranslation_p(inLangPath, &inAppTranslator);
    }
    if (!trLoadSuccess && !isUserLanguageSystem_p())
    {
        trLoadSuccess = loadSystemTranslation_p(inLangPath, &inAppTranslator);
    }
    if (trLoadSuccess)
    {
#ifdef TM_DEBUG
        qDebug() << "installTranslation" << currentLanguage;
#endif
        app->installTranslator(&inAppTranslator);
        if (loadQtTranslation_p(exLangPath, &exQtTranslator))
        {
            app->installTranslator(&exQtTranslator);
        }
        else if (loadQtTranslation_p(inLangPath, &inQtTranslator))
        {
            app->installTranslator(&inQtTranslator);
        }
        QLocale::setDefault(currentLanguage);
        isLangLoaded = true;
    }
#endif
}

QStringList TranslationClass::listTranslations(const QString &langPath)
{
    QDir langDir;
    langDir.setNameFilters(QStringList("tm_*.qm"));
    langDir.setPath(langPath);
    QStringList availableLanguages;
    for (QString lang : langDir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::NoSort))
    {
        availableLanguages << QString(lang).remove("tm_").remove(".qm");
    }
    return availableLanguages;
}

bool TranslationClass::loadSystemTranslation_p(const QString &langPath, QTranslator *appTranslator)
{
#ifdef TM_DEBUG
    qDebug() << "loadSystemTranslation_p";
#endif
    int currentLangCounter = 0;
    for (QString languageName : QLocale::system().uiLanguages())
    {
#ifdef TM_DEBUG
        qDebug() << "loadLanguage" << languageName;
#endif
        QStringList langList = QString(languageName).replace("-","_").split("_");
        if (langList.length() == 2)
        {
#ifdef TM_DEBUG
            qDebug() << "loadLanguageFile" << QString(langPath % "/tm_" % langList.at(0) % "_" % langList.at(1) % ".qm");
#endif
            if (QFile::exists(langPath % "/tm_" % langList.at(0) % "_" % langList.at(1) % ".qm"))
            {
                if (appTranslator->load(langPath % "/tm_" % langList.at(0) % "_" % langList.at(1) % ".qm"))
                {
#ifdef TM_DEBUG
                    qDebug() << "loadLanguageFileSuccess" << QString(langPath % "/tm_" % langList.at(0) % "_" % langList.at(1) % ".qm");
#endif
                    isEnglishMode = false;
                    currentLanguage = languageName;
                    currentLangIndex = currentLangCounter;
                    return true;
                }
            }
#ifdef TM_DEBUG
            qDebug() << "loadLanguageFile" << QString(langPath % "/tm_" % langList.at(0) % ".qm");
#endif
            if (QFile::exists(langPath % "/tm_" % langList.at(0) % ".qm"))
            {
                if (appTranslator->load(langPath % "/tm_" % langList.at(0) % ".qm"))
                {
#ifdef TM_DEBUG
                    qDebug() << "loadLanguageFileSuccess" << QString(langPath % "/tm_" % langList.at(0) % ".qm");
#endif
                    isEnglishMode = false;
                    currentLanguage = languageName;
                    currentLangIndex = currentLangCounter;
                    return true;
                }
                else if (langList.at(0) == "en")
                {
#ifdef TM_DEBUG
                    qDebug() << "languageEnglishMode index" << currentLangCounter;
#endif
                    isEnglishMode = true;
                    currentLanguage = languageName;
                    currentLangIndex = currentLangCounter;
                    return true;
                }
            }
            else if (langList.at(0) == "en")
            {
#ifdef TM_DEBUG
                qDebug() << "languageEnglishMode index" << currentLangCounter;
#endif
                isEnglishMode = true;
                currentLanguage = languageName;
                currentLangIndex = currentLangCounter;
                return true;
            }
        }
        else if (langList.length() == 1)
        {
#ifdef TM_DEBUG
            qDebug() << "loadLanguageFile" << QString(langPath % "/tm_" % langList.at(0) % ".qm");
#endif
            if (QFile::exists(langPath % "/tm_" % langList.at(0) % ".qm"))
            {
                if (appTranslator->load(langPath % "/tm_" % langList.at(0) % ".qm"))
                {
#ifdef TM_DEBUG
                    qDebug() << "loadLanguageFileSuccess" << QString(langPath % "/tm_" % langList.at(0) % ".qm");
#endif
                    isEnglishMode = false;
                    currentLanguage = languageName;
                    currentLangIndex = currentLangCounter;
                    return true;
                }
            }
        }
#ifdef TM_DEBUG
        qDebug() << "currentLangCounter bump";
#endif
        currentLangCounter++;
    }
    return false;
}

bool TranslationClass::loadUserTranslation_p(const QString &langPath, QTranslator *appTranslator)
{
#ifdef TM_DEBUG
    qDebug() << "loadUserTranslation_p";
#endif
    QString languageName = userLanguage;
    QStringList langList = QString(languageName).replace("-","_").split("_");
    if (langList.length() == 2)
    {
#ifdef TM_DEBUG
        qDebug() << "loadLanguageFile" << QString(langPath % "/tm_" % langList.at(0) % "_" % langList.at(1) % ".qm");
#endif
        if (QFile::exists(langPath % "/tm_" % langList.at(0) % "_" % langList.at(1) % ".qm"))
        {
            if (appTranslator->load(langPath % "/tm_" % langList.at(0) % "_" % langList.at(1) % ".qm"))
            {
#ifdef TM_DEBUG
                qDebug() << "loadLanguageFileSuccess" << QString(langPath % "/tm_" % langList.at(0) % "_" % langList.at(1) % ".qm");
#endif
                currentLanguage = languageName;
                return true;
            }
        }
#ifdef TM_DEBUG
        qDebug() << "loadLanguageFile" << QString(langPath % "/tm_" % langList.at(0) % ".qm");
#endif
        if (QFile::exists(langPath % "/tm_" % langList.at(0) % ".qm"))
        {
            if (appTranslator->load(langPath % "/tm_" % langList.at(0) % ".qm"))
            {
#ifdef TM_DEBUG
                qDebug() << "loadLanguageFileSuccess" << QString(langPath % "/tm_" % langList.at(0) % ".qm");
#endif
                currentLanguage = languageName;
                return true;
            }
        }
    }
    else if (langList.length() == 1)
    {
#ifdef TM_DEBUG
        qDebug() << "loadLanguageFile" << QString(langPath % "/tm_" % langList.at(0) % ".qm");
#endif
        if (QFile::exists(langPath % "/tm_" % langList.at(0) % ".qm"))
        {
            if (appTranslator->load(langPath % "/tm_" % langList.at(0) % ".qm"))
            {
#ifdef TM_DEBUG
                qDebug() << "loadLanguageFileSuccess" << QString(langPath % "/tm_" % langList.at(0) % ".qm");
#endif
                currentLanguage = languageName;
                return true;
            }
        }
    }
    return false;
}

bool TranslationClass::loadQtTranslation_p(const QString &langPath, QTranslator *qtTranslator)
{
#ifdef TM_DEBUG
    qDebug() << "loadQtTranslation_p" << currentLanguage;
#endif
    QString languageName = currentLanguage;
    QStringList langList = QString(languageName).replace("-","_").split("_");
    if (langList.length() == 2)
    {
#ifdef TM_DEBUG
        qDebug() << "loadLanguageFile" << QString(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % "_" % langList.at(1) % ".qm");
#endif
        if (QFile::exists(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % "_" % langList.at(1) % ".qm"))
        {
            if (qtTranslator->load(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % "_" % langList.at(1) % ".qm"))
            {
#ifdef TM_DEBUG
                qDebug() << "loadLanguageFileSuccess" << QString(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % "_" % langList.at(1) % ".qm");
#endif
                return true;
            }
        }
#ifdef TM_DEBUG
        qDebug() << "loadLanguageFile" << QString(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % ".qm");
#endif
        if (QFile::exists(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % ".qm"))
        {
            if (qtTranslator->load(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % ".qm"))
            {
#ifdef TM_DEBUG
                qDebug() << "loadLanguageFileSuccess" << QString(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % ".qm");
#endif
                return true;
            }
        }
    }
    else if (langList.length() == 1)
    {
#ifdef TM_DEBUG
        qDebug() << "loadLanguageFile" << QString(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % ".qm");
#endif
        if (QFile::exists(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % ".qm"))
        {
            if (qtTranslator->load(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % ".qm"))
            {
#ifdef TM_DEBUG
                qDebug() << "loadLanguageFileSuccess" << QString(langPath % QDir::separator() % QtBaseTranslationFormat % langList.at(0) % ".qm");
#endif
                return true;
            }
        }
    }
    return false;
}

bool TranslationClass::isUserLanguageSystem_p()
{
    return (userLanguage == "System" || userLanguage.trimmed().isEmpty());
}

QString TranslationClass::getCurrentLanguage()
{
    return currentLanguage;
}

bool TranslationClass::isLanguageLoaded()
{
    return isLangLoaded;
}

void TranslationClass::unloadTranslation(QApplication *app)
{
    if (isLangLoaded)
    {
#ifndef TM_QCONF
        app->removeTranslator(&exAppTranslator);
        app->removeTranslator(&exQtTranslator);
        app->removeTranslator(&inAppTranslator);
        app->removeTranslator(&inQtTranslator);
#else
        app->removeTranslator(&inAppTranslator);
        app->removeTranslator(&exQtTranslator);
#endif
        currentLangIndex = 0;
        currentLanguage = QString();
        QLocale::setDefault(QLocale::c());
        isEnglishMode = false;
        isLangLoaded = false;
    }
#ifdef _MSC_VER // Fix dumb Microsoft compiler warning
    Q_UNUSED(app)
#endif
}

QString TranslationClass::getCountryCode(QLocale::Country country)
{
    QList<QLocale> locales = QLocale::matchingLocales(QLocale::AnyLanguage,
                                                      QLocale::AnyScript,
                                                      country);
    if (locales.isEmpty()) return QString();
    QStringList localeStrList = locales.at(0).name().split("_");
    if (localeStrList.length() <= 2)
    {
        return localeStrList.at(1).toLower();
    }
    else
    {
        return QString();
    }
}

QString TranslationClass::getCountryCode(QLocale locale)
{
    QStringList localeStrList = locale.name().split("_");
    if (localeStrList.length() <= 2)
    {
        return localeStrList.at(1).toLower();
    }
    else
    {
        return QString();
    }
}
