#ifndef MYSOCKET_H
#define MYSOCKET_H


#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkInterface>
#include <QDebug>
#include <QMessageBox>
#include <QChar>
#include <QTime>
#include <QCryptographicHash>
#include "state.h"

class MySocket:QObject
{
    Q_OBJECT
public:
    MySocket(QString ip,QString port);
    //int init(QString ip,QString port);
    ~MySocket();
    int sendText(int type, QString string, QString username, QString aimusername);
    int sendLogin(QString username,QString password);
    int sendRegister(QString username,QString password,QString quest1,QString answ1,
                  QString quest2,QString answ2,QString quest3,QString answ3);
    int sendPwRetrieval(QString username,QString newpassword,QString quest1,QString answ1,
                  QString quest2,QString answ2,QString quest3,QString answ3);
    int sendPwRetrievalAsk(QString username,QStringList *questions);
private slots:
    //void receive();
private:
    QString receiveInfor;
    int connectState;
    QTcpSocket myconnect;
    int checkAndConnect();
};

#endif // MYSOCKET_H
