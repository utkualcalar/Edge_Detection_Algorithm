#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <qstring.h>
#include <qthread.h>

namespace Ui {
    class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    QString getRtspURL();
    Ui::Login *ui;
    QString rtspURL;

private slots:
    void on_btn_play_clicked();
    void on_btn_cancel_clicked();
};
#endif // LOGIN_H
