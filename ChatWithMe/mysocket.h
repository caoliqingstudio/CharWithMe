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
#include "filesr.h"
#include "myspnplus.h"

class MySocket:QObject
{
    Q_OBJECT
public:
    MySocket(QString ip,QString port);
    MySocket(QString ip,QString port,QTcpSocket* insocket);
    QTcpSocket *myconnect;
    //int init(QString ip,QString port);
    ~MySocket();
    int sendText(int type, QString string, QString username, QString aimusername);
    int sendFile(QString filename,QString username,QString aimusername);
    int sendLogin(QString username,QString password);
    int sendRegister(QString username,QString password,QString quest1,QString answ1,
                  QString quest2,QString answ2,QString quest3,QString answ3);
    int sendPwRetrieval(QString username,QString newpassword,QString quest1,QString answ1,
                  QString quest2,QString answ2,QString quest3,QString answ3);
    int sendPwRetrievalAsk(QString username,QStringList *questions);
    int sendConnect(QString username, QStringList*fromnames, QStringList *infors, QStringList *times, QStringList *friends, QStringList *nicknames, QStringList *timefriends);
    int sendAddFriend(QString username, QString aimusername, QString nickname);
    int sendConnect();
    int sendServer(QString aimusername,QString *ip);
    int sendQuit(QString username);//使用 listening
private slots:
    //void receive();
private:

    int connectState;
    QString receiveInfor;
    QString ip,port;
    int checkAndConnect();
};

#endif // MYSOCKET_H
