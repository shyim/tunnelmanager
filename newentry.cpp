/*****************************************************************************
* tunnelmanager - Simple GUI for SSH Tunnels
*
* Copyright (C) 2017 Soner Sayakci
* Copyright (C) 2017 Syping
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*
*****************************************************************************/

#include "newentry.h"
#include "ui_newentry.h"
#include <QDebug>

NewEntry::NewEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEntry)
{
    setWindowFlags(windowFlags()^Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->labelError->setVisible(false);

    // debug
    ui->lineName->setText("");
    ui->lineHost->setText("");
    ui->lineSshPort->setText("22");
    ui->lineLocPort->setText("");
    ui->lineUser->setText("");
    ui->lineExtIP->setText("");
    ui->lineExtPort->setText("");
}

NewEntry::~NewEntry()
{
    delete ui;
}

void NewEntry::adaptSize()
{
    resize(width(), heightForWidth(width()));
}

void NewEntry::entryFailed(QString text)
{
    ui->labelError->setText(tr("<span style=\"color: red;\">%1</span>").arg(text));
    ui->labelError->setVisible(true);
    adaptSize();
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
