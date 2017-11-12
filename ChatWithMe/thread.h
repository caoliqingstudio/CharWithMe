#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QNetworkInterface>
#include <QMap>
#include <iostream>
#include <QtNetwork/QTcpSocket>
#include <QMessageBox>
#include <QTime>
#include <QFile>
#include "myspnplus.h"
#include "state.h"

class Thread: public QThread
{
    Q_OBJECT
public:
    Thread(QString ip,QString filename,QString username,QString aimusername);//0 发文件  1 收文件
    Thread(QTcpSocket *socket, quint64 filesize, QString filename, QString username, QString aimusername);
    ~Thread();
protected:
    char *text;
    bool sendtype;
    quint64 receivefilesize;
    QTcpSocket *mysocket;
    QString *filename;
    QString *username;
    QString *aimusername;
    QString *ip;
    int type;
    virtual void run();
};

#endif // THREAD_H
