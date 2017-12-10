#include "newentry.h"
#include "ui_newentry.h"

NewEntry::NewEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEntry)
{
    setWindowFlags(windowFlags()^Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    // debug
    ui->lineName->setText("test");
    ui->lineHost->setText("shyim.de");
    ui->lineSshPort->setText("22");
    ui->lineLocPort->setText("8555");
    ui->lineUser->setText("root");
    ui->lineExtIP->setText("127.0.0.1");
    ui->lineExtPort->setText("8555");
}

void NewEntry::accept()
{

}

NewEntry::~NewEntry()
{
    delete ui;
}

void NewEntry::on_buttonOk_clicked()
{
    emit newEntryAdded(ui->lineName->text(), ui->lineHost->text(), ui->lineSshPort->text(), ui->lineUser->text(), ui->lineLocPort->text(), ui->lineExtIP->text(), ui->lineExtPort->text());
    close();
}

void NewEntry::on_buttonCancel_clicked()
{
    close();
}
