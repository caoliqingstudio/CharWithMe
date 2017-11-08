#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMainWindow>
#include <QMap>
#include <iostream>
#include "server.h"
#include "addfriend.h"
#include "chat.h"
#include "state.h"

class Thread: public QThread
{
    Q_OBJECT
public:
    Thread(int type);
    //Ui::MainWindow *ui;
    QTcpSocket *myconnect;
    Server *server;
    QMap<QString,Chat*> *chatUserUI;
    QMap<QString,QString> *charUserIP;

protected:
    int type;
    virtual void run();
};

#endif // THREAD_H
