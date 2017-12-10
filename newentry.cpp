#include "newentry.h"
#include "ui_newentry.h"

NewEntry::NewEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEntry)
{
    setWindowFlags(windowFlags()^Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->labelError->setVisible(false);

    // debug
    ui->lineName->setText("test");
    ui->lineHost->setText("shyim.de");
    ui->lineSshPort->setText("22");
    ui->lineLocPort->setText("8555");
    ui->lineUser->setText("root");
    ui->lineExtIP->setText("127.0.0.1");
    ui->lineExtPort->setText("8555");
}

NewEntry::~NewEntry()
{
    delete ui;
}

void NewEntry::entryFailed(QString text)
{
    ui->labelError->setText(tr("<span style=\"color: red;\">%1</span>").arg(text));
    ui->labelError->setVisible(true);
}

void NewEntry::entrySuccess()
{
    ui->labelError->setVisible(false);
    ui->lineName->setText("");
    ui->lineHost->setText("");
    ui->lineSshPort->setText("22");
    ui->lineLocPort->setText("");
    ui->lineUser->setText("");
    ui->lineExtIP->setText("");
    ui->lineExtPort->setText("");
    close();
}

void NewEntry::on_buttonOk_clicked()
{
    if (ui->lineName->text().isEmpty() ||
            ui->lineHost->text().isEmpty() ||
            ui->lineSshPort->text().isEmpty() ||
            ui->lineUser->text().isEmpty() ||
            ui->lineLocPort->text().isEmpty() ||
            ui->lineExtIP->text().isEmpty() ||
            ui->lineExtPort->text().isEmpty())
    {
        entryFailed(tr("Please fill in all fields"));
        return;
    }

    emit newEntryAdded(ui->lineName->text(), ui->lineHost->text(), ui->lineSshPort->text(), ui->lineUser->text(), ui->lineLocPort->text(), ui->lineExtIP->text(), ui->lineExtPort->text(), false);
}

void NewEntry::on_buttonCancel_clicked()
{
    close();
}
