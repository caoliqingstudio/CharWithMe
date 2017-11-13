#include "Server.h"
#include "thread.h"
Server::Server(QObject *parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer();
    //设置最大允许连接数，不设置的话默认为30
    m_tcpServer->setMaxPendingConnections(300);
//    qDebug() << m_tcpServer->maxPendingConnections();
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectSlot()));

}

Server::~Server()
{

}

void Server::init(int port)
{
    this->port=port;
//    m_pMsgHandler = pMsgHandler;

    if(m_tcpServer->listen(QHostAddress::Any, port)){
        qDebug() << "listen OK!";
    }else{
        qDebug() << "listen error!";
    }
}

void Server::newConnectSlot()
{
    QTcpSocket *tcp = m_tcpServer->nextPendingConnection();
    connect(tcp,SIGNAL(readyRead()),this,SLOT(readMessage()));
    m_mapClient.insert(tcp->peerAddress().toString(), tcp);
    connect(tcp,SIGNAL(disconnected()),this,SLOT(removeUserFormList()));
}

void Server::readMessage()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    //qDebug()<<"  this is socket->peerAddress 66\n"<<socket->peerAddress();
    std::string result=socket->readLine().toStdString();
    switch (result.at(0)) {
    case ('0'+CONNECT):{
    }
        break;
    case ('0'+SENDTEXT):{
    }
        break;
    case ('0'+LOGIN)://do nothing
        break;
    case ('0'+REGISTER)://do nothing
        break;
    case ('0'+PWRE)://do nothing
        break;
    case ('0'+ADD):{
    }
        break;
    case ('0'+SENDFILE):{
        QString username=QString::fromStdString(socket->readLine().toStdString());username.chop(1);
        QString friendname=QString::fromStdString(socket->readLine().toStdString());friendname.chop(1);
        QString filename=QString::fromStdString(socket->readLine().toStdString());filename.chop(1);
        QString sizename=QString::fromStdString(socket->readLine().toStdString());sizename.chop(1);
        QString timestr=QString::fromStdString(socket->readLine().toStdString());timestr.chop(1);
        qDebug()<<QString("new file from ")+username+" filesize ="+sizename+timestr;
        //QMessageBox::warning(0,"receive",QString("new file from ")+username+" filesize ="+sizename+timestr);
        Thread *mythread=new Thread(socket,sizename.toLongLong(),filename,username,friendname);
        mythread->fileSR(port);
//        ((MainWindow*)thismainwindow)->byAddChat(username,QString("new file from ")+username+" filesize ="+sizename,timestr);
    }
        break;
    case ('0'+LISTENING)://do nothing

        break;
    case ('0'+PWRE_ASK)://do nothing
        break;
    default:
        break;
    }
}

void Server::removeUserFormList()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    QMap<QString, QTcpSocket *>::iterator it;

    for(it=m_mapClient.begin();it!=m_mapClient.end();it++)
    {
        if(socket->peerAddress().toString() == it.key())
        {
            m_mapClient.erase(it);
            //m_pMsgHandler->devOffline(socket->peerAddress().toString());
            qDebug()<<"duan kai socket->peerAddress().toString() 82\n"<<socket->peerAddress().toString();
            break;
        }
    }
}
