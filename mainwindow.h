#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMap>
#include <QMainWindow>
#include <QListWidgetItem>
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
    Ui::MainWindow *ui;
    NewEntry *entry;
    QMap<QListWidgetItem*, QProcess*> processMap;
    QMap<QListWidgetItem*, QStringList> itemData;
    QStringList buildPlinkOptions(QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort);

private slots:
    void addNewEntry();
    void entryDialogClosed(int id);
    void on_action_ber_Qt_triggered();
    void on_actionBeenden_triggered();
    void newEntryAdded(QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort);
    void on_buttonPlink_triggered();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
