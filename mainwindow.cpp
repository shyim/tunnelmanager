#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newentry.h"
#include <QSystemTrayIcon>
#include <QStringBuilder>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>
#include <QProcess>
#include <QDialog>
#include <QDebug>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    entry = nullptr;
    ui->setupUi(this);

    QSettings settings ("Shyim", "SSH Tunnel Manager");

#ifdef Q_OS_WIN
    settings.beginGroup("settings");
    ui->linePlink->setText(settings.value("plink").toString());
    settings.endGroup();
#else
    ui->linePlink->setVisible(false);
    ui->linePlink->setText("ssh");
#endif

    settings.beginGroup("hosts");
    for (QString name : settings.allKeys())
    {
        QStringList dataList = settings.value(name, QStringList()).toStringList();
        newEntryAdded(name, dataList.at(0), dataList.at(1), dataList.at(2), dataList.at(3), dataList.at(4), dataList.at(5));
    }
    settings.endGroup();

    trayIcon = new QSystemTrayIcon(QIcon(":/server.png"), this);
    trayIcon->setToolTip(tr("Tunnel Manager"));
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(toggleWindowState()));
}

void MainWindow::addNewEntry()
{
#ifdef Q_OS_WIN
    if (ui->linePlink->text().isEmpty() || !QFile::exists(ui->linePlink->text()))
    {
        QMessageBox::information(this, tr("Error"), tr("No plink selected"));
        return;
    }
#endif

    if (entry == nullptr)
    {
        entry = new NewEntry(this);
        connect(entry, SIGNAL(newEntryAdded(QString,QString,QString,QString,QString,QString,QString,bool)), this, SLOT(newEntryAdded(QString,QString,QString,QString,QString,QString,QString,bool)));
        connect(entry, SIGNAL(finished(int)), this, SLOT(entryDialogClosed(int)));
    }
    entry->show();
}

void MainWindow::newEntryAdded(QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort, bool startup)
{
    QString plink = ui->linePlink->text();
    QStringList plink_args = buildPlinkOptions(host, sshPort, user, locPort, extIP, extPort);

    if (!startup)
    {
        bool duplicate = false;
        for (QStringList stringList : itemData)
        {
            if (stringList.length() >= 1)
            {
                if (name == stringList.at(0))
                {
                    duplicate = true;
                    break;
                }
            }
        }

        if (duplicate)
        {
            entry->entryFailed(tr("%1 already exists").arg(name));
            return;
        }
        else
        {
            entry->entrySuccess();
        }
    }

    QListWidgetItem *newItem = new QListWidgetItem();
    itemData[newItem] << name << host << sshPort << user << locPort << extIP << extPort;

    QString eStr;
    for (QString str : itemData[newItem])
    {
        eStr += str % " ";
    }
    eStr = eStr.trimmed();
    newItem->setText(eStr);

    ui->listWidget->addItem(newItem);

    QProcess *process = new QProcess;
    process->start(plink, plink_args);

    processMap[newItem] = process;
}

void MainWindow::entryDialogClosed(int id)
{
    Q_UNUSED(id)
    delete entry;
    entry = nullptr;
}

MainWindow::~MainWindow()
{
    QSettings settings("Shyim", "SSH Tunnel Manager");
#ifdef Q_OS_WIN
    settings.beginGroup("settings");
    settings.setValue("plink", ui->linePlink->text());
    settings.endGroup();
#endif
    settings.beginGroup("hosts");
    QStringList nameList;
    for (QStringList stringList : itemData.values())
    {
        QStringList dataList = stringList;
        dataList.removeAt(0);
        settings.setValue(stringList.at(0), dataList);
        nameList << stringList.at(0);
    }
    for (QString name : settings.allKeys())
    {
        if (!nameList.contains(name))
        {
            settings.remove(name);
        }
    }
    settings.endGroup();

    // stop runing plink
    for (QProcess *plink : processMap)
    {
        plink->kill();
    }

    trayIcon->hide();
    delete trayIcon;

    delete ui;
}

void MainWindow::on_actionBeenden_triggered()
{
    QApplication::quit();
}

void MainWindow::on_buttonPlink_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, tr("plink auswählen"), QString(), "plink.exe (plink.exe)");
    if (path != "")
    {
        ui->linePlink->setText(path);
    }
}

#ifdef Q_OS_WIN
QStringList MainWindow::buildPlinkOptions(QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort)
{
    QStringList plink_args;

    plink_args += "-ssh";
    plink_args += "-P";
    plink_args += sshPort;
    plink_args += "-L";
    plink_args += locPort % ":" % extIP % ":" % extPort;
    plink_args += user % "@" % host;

    return plink_args;
}
#else
QStringList MainWindow::buildPlinkOptions(QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort)
{
    QStringList plink_args;

    plink_args += "-f";
    plink_args += user % "@" % host % ":" % sshPort;
    plink_args += "-L";
    plink_args += locPort % ":" % extIP % ":" % extPort;
    plink_args += "-N";

    return plink_args;
}
#endif

void MainWindow::on_pushButton_2_clicked()
{
    for (QListWidgetItem *item : ui->listWidget->selectedItems())
    {
        processMap[item]->kill();
        processMap.remove(item);
        itemData.remove(item);
        delete item;
    }
}

void MainWindow::toggleWindowState()
{
    if (this->isVisible()) {
        this->setVisible(false);
    } else {
        this->setVisible(true);
    }
}
