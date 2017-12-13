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

    QSettings settings ("Shyim", "SSH Tunnel Manager");

#ifdef Q_OS_WIN
    settings.beginGroup("settings");
    ui->linePlink->setText(settings.value("plink").toString());
    settings.endGroup();
#else
    ui->linePlink->setVisible(false);
    ui->buttonPlink->setVisible(false);
    ui->labelPlink->setVisible(false);
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
    entry->adaptSize();
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
    connect(process, SIGNAL(started()), this, SLOT(onTunnelStart()));
    connect(process, SIGNAL(finished(int)), this, SLOT(onTunnelCrash(int)));
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
        disconnect(plink, SIGNAL(started()), this, SLOT(onTunnelStart()));
        disconnect(plink, SIGNAL(finished(int)), this, SLOT(onTunnelCrash(int)));
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
    QString path = QFileDialog::getOpenFileName(this, tr("Select plink.exe"), QString(), "plink.exe (plink.exe)");
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

    plink_args += "-o";
    plink_args += "StrictHostKeyChecking=no";
    plink_args += "-o";
    plink_args += "UserKnownHostsFile=/dev/null ";

    plink_args += user % "@" % host;
    plink_args += "-p";
    plink_args += sshPort;
    plink_args += "-L";
    plink_args += locPort % ":" % extIP % ":" % extPort;
    plink_args += "-N";

    return plink_args;
}
#endif

void MainWindow::on_buttonDelete_clicked()
{
    for (QTreeWidgetItem *item : ui->treeWidget->selectedItems())
    {
        deleteTreeWidgetItem(item);
    }
}

void MainWindow::deleteTreeWidgetItem(QTreeWidgetItem *item)
{
    processToWidgetItem.remove(processMap[item]);
    processMap[item]->kill();
    processMap.remove(item);
    itemData.remove(item);
    delete item;
}

void MainWindow::toggleWindowState()
{
    if (this->isVisible()) {
        this->setVisible(false);
    } else {
        this->setVisible(true);
    }
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *contextMenu = new QMenu();
    QAction *deleteAction = contextMenu->addAction(tr("Delete"), this, SLOT(deleteFromContextMenu()));
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
    QProcess *process = dynamic_cast<QProcess*>(sender());

    processToWidgetItem[process]->setText(0, tr("Running"));
    processToWidgetItem[process]->setTextColor(0, QColor("green"));
}

void MainWindow::onTunnelCrash(int exitCode)
{
    QProcess *process = dynamic_cast<QProcess*>(sender());

    qDebug() << exitCode;

    processToWidgetItem[process]->setText(0, process->readAllStandardError());
    processToWidgetItem[process]->setTextColor(0, QColor("red"));
}
