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

#ifndef NEWENTRY_H
#define NEWENTRY_H

#include <QDialog>
#include <QStringList>
#include <QTreeWidgetItem>

namespace Ui {
class NewEntry;
}

class NewEntry : public QDialog
{
    Q_OBJECT

public:
    explicit NewEntry(QWidget *parent = 0);
    void fillForm(QStringList data, QTreeWidgetItem *item);
    void entryFailed(QString text);
    void entrySuccess();
    void adaptSize();
    ~NewEntry();

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();

signals:
    void newEntryAdded(QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort, bool startup);
    void itemModified(QTreeWidgetItem *item, QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort);

private:
    Ui::NewEntry *ui;
    QTreeWidgetItem *item;
};

#endif // NEWENTRY_H
