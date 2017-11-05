#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMainWindow>
#include <QMap>
#include <iostream>
#include "server.h"
#include "state.h"
#include "mysocket.h"

class Thread: public QThread
{
    Q_OBJECT
public:
    Thread();
    QTcpSocket *myconnect;
    QMap<QString, QTcpSocket *> m_mapClient;
    QMap<QString,Thread *> m_mapThread;

protected:
    virtual void run();
};

#endif // THREAD_H
