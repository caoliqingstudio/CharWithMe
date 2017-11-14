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

class FileSR
{
public:
    FileSR();
    bool fileSend(QTcpSocket *socket,QString filename, QString username, QString aimusername);
    bool fileReceive(QTcpSocket *socket, quint64 filesize, QString filename, QString username, QString aimusername);
};

#endif // FILESR_H
