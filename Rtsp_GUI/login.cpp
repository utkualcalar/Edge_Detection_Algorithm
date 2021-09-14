//Yasar Utku Alcalar

#include "login.h"
#include "ui_login.h"
#include <qstring.h>
#include <stdio.h>
#include <QString>
#include "mainwindow.h"
#include <stdlib.h>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_btn_play_clicked()
{
    rtspURL = ui->rtspURL->text();
    close();
}

QString Login::getRtspURL()
{
    return rtspURL;
}

void Login::on_btn_cancel_clicked()
{
    exit(0);
}
