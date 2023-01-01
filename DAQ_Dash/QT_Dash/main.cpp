#include "mainwindow.h"
#include "worker.h"

#include <QApplication>
#include <QThread>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Worker work;
    w.show();


    QThread threadA;
    threadA.setObjectName("A");
    work.moveToThread(&threadA);
    QObject::connect(&work, &Worker::produced, &w, &MainWindow::receive);

    //cout <<"start";

    QObject::connect(&threadA, &QThread::started, &work, &Worker::start);
    threadA.start();
    return a.exec();
}
