#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();

signals:
    void produced(int value);
    void finished();

public slots:
    void start();
    void quit();

private:
    int m_count;

    void timeout();
};

#endif // WORKER_H
