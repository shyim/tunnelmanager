#ifndef NEWENTRY_H
#define NEWENTRY_H

#include <QDialog>

namespace Ui {
class NewEntry;
}

class NewEntry : public QDialog
{
    Q_OBJECT

public:
    explicit NewEntry(QWidget *parent = 0);
    void entryFailed(QString text);
    void entrySuccess();
    void adaptSize();
    ~NewEntry();

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();

signals:
    void newEntryAdded(QString name, QString host, QString sshPort, QString user, QString locPort, QString extIP, QString extPort, bool startup);

private:
    Ui::NewEntry *ui;
};

#endif // NEWENTRY_H
