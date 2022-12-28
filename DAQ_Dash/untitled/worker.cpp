#include "worker.h"

Worker::Worker()
{

}

void Worker::start()
{
    for(int x = 0; x < 10; x++){
        qInfo() << this << "sent " << x << " " << QThread::currentThread();
        emit produced(x);
        QThread::currentThread()->msleep(1000);
    }
    quit();
}

void Worker::quit()
{
    qInfo() << this << "quitting" << QThread::currentThread();
    emit finished();
}
