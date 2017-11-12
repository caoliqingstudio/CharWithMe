#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QString::fromLocal8Bit ("中文");
    QApplication a(argc, argv);
    Login login;
    QString username,ip,port;
    login.ip=&ip;
    login.port=&port;
    login.mainUserName=&username;
    login.init();
    login.show();
    //return a.exec();
    int returnResult=a.exec();
    MySocket mysocket(ip,port);
    mysocket.sendQuit(username);
    return returnResult;
}
