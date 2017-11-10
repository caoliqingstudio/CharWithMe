#ifndef MYSERVER_H
#define MYSERVER_H

#include <iostream>
#include <string>
#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkInterface>
#include <QDebug>
#include <QTime>
#include "ui_mainwindow.h"
#include "state.h"
#include "thread.h"
#include "mainwindow.h"
#include "database.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    Ui::MainWindow *ui;
    void init(int port);
signals:


private slots:

    void newConnectSlot();
    void readMessage();
    void removeUserFormList();
private:
    DataBase db;
    QTcpServer *m_tcpServer;
    QMap<QString, QTcpSocket *> m_mapClient;
    QMap<QString, void *> m_mapThread;//本来应该是  Thread * 报错 可能是格式问题
    bool stateQmapClient;
    bool stateQmapThread;

//    IMsgInterface *m_pMsgHandler;
};

#endif // MYSERVER_H
