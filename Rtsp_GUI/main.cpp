//Yasar Utku Alcalar

#include "mainwindow.h"
#include "ffmpegdecoder.h"
#include <QApplication>
#include "login.h"
#include <QWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
