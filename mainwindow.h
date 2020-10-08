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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMap>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QStringList>
#include "newentry.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void adaptSize();

private:
    QVector<QString> buildPlinkOpenSSHOptions(const QString &exec, const QString &host, const QString &sshPort, const QString &user, const QString &locPort, const QString &extIP, const QString &extPort);
    void deleteTreeWidgetItem(QTreeWidgetItem *item);

    Ui::MainWindow *ui;
    NewEntry *entry;
    QMap<QTreeWidgetItem*, QProcess*> processMap;
    QMap<QTreeWidgetItem*, QStringList> itemData;
    QMap<QProcess*, QTreeWidgetItem*> processToWidgetItem;
    QSystemTrayIcon *trayIcon;

public slots:
    void itemModified(QTreeWidgetItem *item, QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort);

private slots:
    void addNewEntry();
    void entryDialogClosed(int id);
    void on_actionQuit_triggered();
    void newEntryAdded(QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort, bool startup = true);
    void on_buttonDelete_clicked();
    void selectPlinkOpenSSH();
    void toggleWindowState();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void deleteFromContextMenu();
    void onTunnelStart();
    void onTunnelCrash(int exitCode);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
};

#endif // MAINWINDOW_H
