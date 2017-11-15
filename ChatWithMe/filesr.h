#ifndef FILESR_H
#define FILESR_H


//#include <QMainWindow>
//#include <QWidget>
//#include <QDialog>
//#include <QLabel>
//#include <QHBoxLayout>
#include <QtNetwork/QTcpServer>
//#include <QtNetwork/QNetworkInterface>
//#include <QMap>
//#include <iostream>
#include <QtNetwork/QTcpSocket>
#include <QMessageBox>
#include <QTime>
#include <QFile>
#include "myspnplus.h"
#include "state.h"

#define BLOCK_SIZE 1024

class FileSR
{
public:
    FileSR();
    bool fileSend(QTcpSocket *socket,QString filename, QString username, QString aimusername);
    bool fileReceive(QTcpSocket *mysocket, quint64 filesize, QString filename, QString username, QString aimusername);
    bool fileSendFast(QTcpSocket *socket,QString filename, QString username, QString aimusername);
    bool fileReceiveFast(QTcpSocket *mysocket, quint64 filesize, QString filename, QString username, QString aimusername);
    bool fileSendFast_A(QTcpSocket *socket,QString filename, QString username, QString aimusername);
    bool fileReceiveFast_A(QTcpSocket *mysocket, quint64 filesize, QString filename, QString username, QString aimusername);
};

#endif // FILESR_H
