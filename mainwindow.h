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

private:
    QStringList buildPlinkOptions(QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort);
    void deleteTreeWidgetItem(QTreeWidgetItem *item);

    //
    Ui::MainWindow *ui;
    NewEntry *entry;
    QMap<QTreeWidgetItem*, QProcess*> processMap;
    QMap<QTreeWidgetItem*, QStringList> itemData;
    QSystemTrayIcon *trayIcon;

private slots:
    void addNewEntry();
    void entryDialogClosed(int id);
    void on_actionBeenden_triggered();
    void newEntryAdded(QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort, bool startup = true);
    void on_buttonPlink_triggered();
    void on_buttonDelete_clicked();
    void toggleWindowState();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void deleteFromContextMenu();
};

#endif // MAINWINDOW_H
