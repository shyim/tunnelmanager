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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newentry.h"
#include "config.h"
#include <QAbstractScrollArea>
#include <QSystemTrayIcon>
#include <QStringBuilder>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>
#include <QProcess>
#include <QObject>
#include <QDialog>
#include <QDebug>
#include <QMenu>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    entry = nullptr;
    ui->setupUi(this);

    QSettings settings(TM_APPVENDOR, TM_APPSTR);
#ifdef Q_OS_WIN
    settings.beginGroup("Settings");
    ui->linePlinkOpenSSH->setText(settings.value("PlinkOpenSSH").toString());
    settings.endGroup();
    QObject::connect(ui->buttonPlinkOpenSSH, &QToolButton::clicked, this, &MainWindow::selectPlinkOpenSSH);
#else
    ui->linePlinkOpenSSH->setVisible(false);
    ui->buttonPlinkOpenSSH->setVisible(false);
    ui->labelPlinkOpenSSH->setVisible(false);
    ui->linePlinkOpenSSH->setText("ssh");
#endif
    ui->actionQuit->setShortcut(QKeySequence("Alt+F4"));

    settings.beginGroup("hosts");
    for (const QString &name : settings.allKeys()) {
        QStringList dataList = settings.value(name, QStringList()).toStringList();
        newEntryAdded(name, dataList.at(0), dataList.at(1), dataList.at(2), dataList.at(3), dataList.at(4), dataList.at(5));
    }
    settings.endGroup();

    QIcon serverIcon(":/server.png");
    trayIcon.setIcon(serverIcon);
    trayIcon.setToolTip(QString::fromUtf8(TM_APPSTR));
    trayIcon.show();
    setWindowIcon(serverIcon);

    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::toggleWindowState);

    adaptSize();
}

void MainWindow::adaptSize()
{
    resize(550, 450);
}

void MainWindow::addNewEntry()
{
#ifdef Q_OS_WIN
    if (ui->linePlinkOpenSSH->text().isEmpty() || !QFile::exists(ui->linePlinkOpenSSH->text())) {
        QMessageBox::information(this, tr("Error"), tr("No Plink/OpenSSH selected"));
        return;
    }
#endif

    if (entry == nullptr) {
        entry = new NewEntry(this);
        QObject::connect(entry, &NewEntry::newEntryAdded, this, &MainWindow::newEntryAdded);
        QObject::connect(entry, &NewEntry::itemModified, this, &MainWindow::itemModified);
        QObject::connect(entry, &NewEntry::finished, this, &MainWindow::entryDialogClosed);
    }
    entry->show();
    entry->adaptSize();
}

void MainWindow::newEntryAdded(QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort, bool startup)
{
    const QString plinkOpenSSH = ui->linePlinkOpenSSH->text();
    const QStringList plinkOpenSSH_args = buildPlinkOpenSSHOptions(plinkOpenSSH, host, sshPort, user, locPort, extIP, extPort).toList();

    if (!startup) {
        bool duplicate = false;
        for (const QStringList &stringList : qAsConst(itemData)) {
            if (stringList.length() >= 1) {
                if (name == stringList.at(0)) {
                    duplicate = true;
                    break;
                }
            }
        }

        if (duplicate) {
            entry->entryFailed(tr("%1 already exists").arg(name));
            return;
        }
        else {
            entry->entrySuccess();
            entry->close();
        }
    }

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    itemData[newItem] << name << host << sshPort << user << locPort << extIP << extPort;

    newItem->setText(0, tr("Not Running"));
    newItem->setTextColor(0, QColor("red"));
    newItem->setText(1, name);
    newItem->setText(2, host);
    newItem->setText(3, locPort);
    newItem->setText(4, extPort);

    ui->treeWidget->addTopLevelItem(newItem);

    QProcess *process = new QProcess;
    processToWidgetItem[process] = newItem;
    QObject::connect(process, &QProcess::started, this, &MainWindow::onTunnelStart);
    QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onTunnelCrash);
    process->start(plinkOpenSSH, plinkOpenSSH_args);

    processMap[newItem] = process;
}

void MainWindow::entryDialogClosed(int id)
{
    Q_UNUSED(id)
    entry->deleteLater();
    entry = nullptr;
}

MainWindow::~MainWindow()
{
    QSettings settings(TM_APPVENDOR, TM_APPSTR);
#ifdef Q_OS_WIN
    settings.beginGroup("Settings");
    settings.setValue("PlinkOpenSSH", ui->linePlinkOpenSSH->text());
    settings.endGroup();
#endif
    settings.beginGroup("hosts");
    QStringList nameList;
    for (const QStringList &stringList : qAsConst(itemData)) {
        QStringList dataList = stringList;
        dataList.removeFirst();
        settings.setValue(stringList.at(0), dataList);
        nameList << stringList.at(0);
    }
    for (const QString &name : settings.allKeys()) {
        if (!nameList.contains(name))
            settings.remove(name);
    }
    settings.endGroup();

    // stop runing Plink/OpenSSH
    for (QProcess *plinkOpenSSH : qAsConst(processMap)) {
        QObject::disconnect(plinkOpenSSH, &QProcess::started, this, &MainWindow::onTunnelStart);
        QObject::disconnect(plinkOpenSSH, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onTunnelCrash);
        plinkOpenSSH->kill();
    }

    trayIcon.hide();
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    trayIcon.hide();
    QApplication::quit();
}

void MainWindow::selectPlinkOpenSSH()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select plink.exe or ssh.exe"), QString(), "Plink/OpenSSH (plink.exe ssh.exe)");
    if (path != "") {
        ui->linePlinkOpenSSH->setText(path);
    }
}

#ifdef Q_OS_WIN
QVector<QString> MainWindow::buildPlinkOpenSSHOptions(const QString &exec, const QString &host, const QString &sshPort, const QString &user, const QString &locPort, const QString &extIP, const QString &extPort)
{
    QVector<QString> plinkOpenSSH_args;

    if (QFileInfo(exec).baseName() == "plink") {
        plinkOpenSSH_args += "-ssh";
        plinkOpenSSH_args += "-P";
        plinkOpenSSH_args += sshPort;
        plinkOpenSSH_args += "-L";
        plinkOpenSSH_args += locPort % ":" % extIP % ":" % extPort;
        plinkOpenSSH_args += user % "@" % host;
    }
    else {
        plinkOpenSSH_args += "-o";
        plinkOpenSSH_args += "StrictHostKeyChecking=no";
        plinkOpenSSH_args += "-o";
        plinkOpenSSH_args += "UserKnownHostsFile=/dev/null ";

        plinkOpenSSH_args += user % "@" % host;
        plinkOpenSSH_args += "-p";
        plinkOpenSSH_args += sshPort;
        plinkOpenSSH_args += "-L";
        plinkOpenSSH_args += locPort % ":" % extIP % ":" % extPort;
        plinkOpenSSH_args += "-N";
    }

    return plinkOpenSSH_args;
}
#else
QVector<QString> MainWindow::buildPlinkOpenSSHOptions(const QString &exec, const QString &host, const QString &sshPort, const QString &user, const QString &locPort, const QString &extIP, const QString &extPort)
{
    Q_UNUSED(exec)
    QVector<QString> openSSH_args;

    openSSH_args += "-o";
    openSSH_args += "StrictHostKeyChecking=no";
    openSSH_args += "-o";
    openSSH_args += "UserKnownHostsFile=/dev/null ";

    openSSH_args += user % "@" % host;
    openSSH_args += "-p";
    openSSH_args += sshPort;
    openSSH_args += "-L";
    openSSH_args += locPort % ":" % extIP % ":" % extPort;
    openSSH_args += "-N";

    return openSSH_args;
}
#endif

void MainWindow::on_buttonDelete_clicked()
{
    for (QTreeWidgetItem *item : ui->treeWidget->selectedItems()) {
        deleteTreeWidgetItem(item);
    }
}

void MainWindow::deleteTreeWidgetItem(QTreeWidgetItem *item)
{
    processToWidgetItem.remove(processMap[item]);
    processMap[item]->disconnect(); // disconnect slots to avoid crash at onTunnelCrash
    processMap[item]->kill();
    processMap.remove(item);
    itemData.remove(item);
    delete item;
}

void MainWindow::toggleWindowState()
{
    if (isMinimized() || !isVisible())
        showNormal();
    else
        hide();
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *contextMenu = new QMenu;
    QAction *deleteAction = contextMenu->addAction(tr("Delete"), this, &MainWindow::deleteFromContextMenu);
    contextMenu->exec(ui->treeWidget->mapToGlobal(pos));
    delete deleteAction;
    delete contextMenu;
}

void MainWindow::deleteFromContextMenu()
{
    on_buttonDelete_clicked();
}

void MainWindow::onTunnelStart()
{
    QProcess *process = static_cast<QProcess*>(sender());

    processToWidgetItem[process]->setText(0, tr("Running"));
    processToWidgetItem[process]->setTextColor(0, QColor("green"));

    ui->statusBar->showMessage(tr("%1 started").arg(processToWidgetItem[process]->text(1)));
}

void MainWindow::onTunnelCrash(int exitCode, QProcess::ExitStatus exitStatus)
{
    QProcess *process = static_cast<QProcess*>(sender());

    ui->statusBar->showMessage(tr("%1 quited with Exit Code %2").arg(processToWidgetItem.value(process)->text(1), QString::number(exitCode)));

    processToWidgetItem[process]->setText(0, process->readAllStandardError());
    processToWidgetItem[process]->setTextColor(0, QColor("red"));
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    addNewEntry();

    entry->fillForm(itemData[item], item);
}

void MainWindow::itemModified(QTreeWidgetItem *item, QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort)
{
    QProcess *process = processMap.value(item, nullptr);
    if (process != nullptr) {
        processToWidgetItem.remove(process);
        processMap.remove(item);
        process->disconnect();
        process->kill();
        process->waitForFinished();
        process->deleteLater();
    }

    itemData[item].clear();
    itemData[item] << name << host << sshPort << user << locPort << extIP << extPort;

    item->setText(0, tr("Not Running"));
    item->setTextColor(0, QColor("red"));
    item->setText(1, name);
    item->setText(2, host);
    item->setText(3, locPort);
    item->setText(4, extPort);

    const QString plinkOpenSSH = ui->linePlinkOpenSSH->text();
    const QStringList plinkOpenSSH_args = buildPlinkOpenSSHOptions(plinkOpenSSH, host, sshPort, user, locPort, extIP, extPort).toList();

    process = new QProcess;
    processToWidgetItem[process] = item;
    QObject::connect(process, &QProcess::started, this, &MainWindow::onTunnelStart);
    QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onTunnelCrash);
    process->start(plinkOpenSSH, plinkOpenSSH_args);

    processMap[item] = process;

    entry->close();
}
