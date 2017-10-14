#include "mainwindow.h"
#include "ui_mainwindow.h"
bool threadend = false;
bool saveData = false;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cap(new Capture())
{
    ui->setupUi(this);
    cap->moveToThread(&capThread);
    connect(&capThread,SIGNAL(finished()),cap,SLOT(deleteLater()));
    connect(&capThread,SIGNAL(started()),cap,SLOT(loop()));
    connect(cap,SIGNAL(setImage(QImage)),this,SLOT(getImage(QImage)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_clicked()
{
    if(!capThread.isRunning())
    {
        capThread.start();
    }
}

void MainWindow::on_stop_clicked()
{
    if(capThread.isRunning())
    {
        threadend = true;
        capThread.quit();
        capThread.wait();
    }
}

void MainWindow::on_save_clicked()
{
    saveData = true;
}
void MainWindow::getImage(QImage img)
{
    QImage& imgshow = img;
    imgshow.scaled(ui->imgcap->width(),ui->imgcap->height());
    ui->imgcap->setPixmap(QPixmap::fromImage(imgshow));
}
