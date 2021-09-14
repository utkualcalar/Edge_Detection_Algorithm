//Yasar Utku Alcalar

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "ffmpegdecoder.h"
#include <opencv2/opencv.hpp>
#include <QPainter>
#include <QDebug>
#include <QElapsedTimer>
#include <unistd.h>
#include <QString>
#include "login.h"
#include <QInputDialog>


using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    Login l;
    l.exec();
    decoder = new FFmpegDecoder(l.rtspURL.toStdString());

    decoder->connect();
    mode = 1; //starts with Sobelx edge detection
    if(decoder->isConnected()) {
        new std::thread(&FFmpegDecoder::decode, decoder);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{

    static QElapsedTimer t;
    if(decoder->isConnected())
    {
        cv::Mat src;

        decoder->mtx.lock();
        if(!decoder->decodedImgBuf.empty())
        {
            src = decoder->decodedImgBuf.front().clone();
            decoder->decodedImgBuf.pop_front();
        }
        decoder->mtx.unlock();


        if(!src.empty())
        {

            cv::Mat img_gray;
            cv::cvtColor(src, img_gray, cv::COLOR_RGB2GRAY);
            cv::Mat img_blur;

            GaussianBlur(img_gray, img_blur, cv::Size(3,3), 0); // Blur the image for better edge detection

            cv::Mat sobelx, sobely, sobelxy;
            cv::Mat edges;

            Sobel(img_blur, sobelx, CV_64F, 1, 0, 5); // Sobelx edge detection
            convertScaleAbs(sobelx, sobelx); // Scales, calculates absolute values, and converts the result to 8-bit

            Sobel(img_blur, sobely, CV_64F, 0, 1, 5); // Sobely edge detection
            convertScaleAbs(sobely, sobely); // Scales, calculates absolute values, and converts the result to 8-bit

            Sobel(img_blur, sobelxy, CV_64F, 1, 1, 5); // Sobelxy edge detection
            convertScaleAbs(sobelxy, sobelxy); // Scales, calculates absolute values, and converts the result to 8-bit

            Canny(img_blur, edges, 100, 200, 3, false); // Canny edge detection

            uchar *data;
            // Display edge detection images
                 if   (mode ==1) {
                 data = sobelx.data;
            } else if (mode ==2) {
                 data = sobely.data;
            } else if (mode ==3) {
                 data = sobelxy.data;
            } else if (mode ==4) {
                 data = edges.data;
            } else {
                src.release();
                update();
                printf("Invalid number");
                return; }

            QPainter painter(this);
            QImage image = QImage(data, sobelx.cols, sobelx.rows, QImage::Format_Grayscale8);
            QPixmap  pix =  QPixmap::fromImage(image);
            painter.drawPixmap(0, 0, ui->widget->width(), ui->widget->height(), pix);
            if (t.elapsed())
                painter.drawText(10, 10, QString("FPS: %1").arg(1000/t.restart()));
            update();
            usleep(20000);
        }

        src.release();
    }
}


void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setStyleSheet("QPushButton{ background-color: yellow }");
    ui->pushButton_2->setStyleSheet("QPushButton{ background-color: white }");
    ui->pushButton_3->setStyleSheet("QPushButton{ background-color: white }");
    ui->pushButton_4->setStyleSheet("QPushButton{ background-color: white }");
    mode= 1;
}

void MainWindow::on_pushButton_2_clicked()
{   ui->pushButton->setStyleSheet("QPushButton{ background-color: white }");
    ui->pushButton_2->setStyleSheet("QPushButton{ background-color: yellow }");
    ui->pushButton_3->setStyleSheet("QPushButton{ background-color: white }");
    ui->pushButton_4->setStyleSheet("QPushButton{ background-color: white }");
    mode= 2;
}

void MainWindow::on_pushButton_3_clicked()
{   ui->pushButton->setStyleSheet("QPushButton{ background-color: white }");
    ui->pushButton_2->setStyleSheet("QPushButton{ background-color: white }");
    ui->pushButton_3->setStyleSheet("QPushButton{ background-color: yellow }");
    ui->pushButton_4->setStyleSheet("QPushButton{ background-color: white }");
    mode= 3;
}

void MainWindow::on_pushButton_4_clicked()
{   ui->pushButton->setStyleSheet("QPushButton{ background-color: white }");
    ui->pushButton_2->setStyleSheet("QPushButton{ background-color: white }");
    ui->pushButton_3->setStyleSheet("QPushButton{ background-color: white }");
    ui->pushButton_4->setStyleSheet("QPushButton{ background-color: yellow }");
    mode= 4;
}
