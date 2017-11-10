#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkInterface>
#include <QDebug>
#include "chat.h"
#include "state.h"
//#include "IMsgInterface.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    QMap<QString,Chat*> *chatUserUI;
    QMap<QString,QString> *charUserIP;
    void init(int port);
signals:


private slots:
    void newConnectSlot();
    void readMessage();
    void removeUserFormList();
private:
    QTcpServer *m_tcpServer;
    QMap<QString, QTcpSocket *> m_mapClient;
};
#endif // SERVER_H
