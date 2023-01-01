#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receive(int data)
{
    qInfo() << this << "receive " << data << " " << QThread::currentThread();
    ui->progressBar->setValue(data*10);
}

