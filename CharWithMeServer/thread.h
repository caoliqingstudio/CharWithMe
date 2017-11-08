#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMainWindow>
#include <QMap>
#include <iostream>
#include <QtNetwork/QTcpSocket>
#include "myserver.h"
#include "state.h"
#include "mysocket.h"
#include "mainwindow.h"

class Thread: public QThread
{
    Q_OBJECT
public:
    Thread();
    int type;
    QTcpSocket *myconnect;
    void * myserver;//本来打算用 server
    QMap<QString, QTcpSocket *> m_mapClient;
    QMap<QString,void *> m_mapThread;

    Ui::MainWindow *ui;
protected:
    virtual void run();
};

#endif // THREAD_H
