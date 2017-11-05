#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkInterface>
#include <QDebug>
#include "state.h"
#include "thread.h"

//#include "IMsgInterface.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();

    void init(int port);
//    void init(IMsgInterface * pMsgHandler, int port);
//    void sendData(QString ip, QString data);
//    void sendData(QString ip, uchar *rawData);
signals:


private slots:

    void newConnectSlot();
    void readMessage();
    void removeUserFormList();
private:
    QTcpServer *m_tcpServer;
    QMap<QString, QTcpSocket *> m_mapClient;
    QMap<QString, void *> m_mapThread;//本来应该是  Thread * 报错 可能是格式问题

//    IMsgInterface *m_pMsgHandler;
};
#endif // SERVER_H
