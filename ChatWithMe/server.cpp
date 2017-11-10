#include "Server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer();
    //设置最大允许连接数，不设置的话默认为30
    m_tcpServer->setMaxPendingConnections(30);
//    qDebug() << m_tcpServer->maxPendingConnections();
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectSlot()));

}

Server::~Server()
{

}

void Server::init(int port)
{
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
    //m_pMsgHandler->recvFromServer(socket->peerAddress().toString(), QString(socket->readAll()));

    qDebug()<<"  this is socket->peerAddress 66\n"<<socket->peerAddress();
//    std::string result=socket->readLine().toStdString();
//    for(int i=0;i<result.length();i++){
//        std::cout<<"this is "<<i<<" : ";
//        std::cout<<result.at(i);
//        std::cout<<"  end"<<std::endl;
//    }
//    result=socket->readLine().toStdString();
//    for(int i=0;i<result.length();i++){
//        std::cout<<"that is "<<i<<" : ";
//        std::cout<<result.at(i);
//        std::cout<<"  end"<<std::endl;
//    }
    qDebug()<<"  this is socket->readAll 67\n"<<QString(socket->readAll());
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
            qDebug()<<"this is socket->peerAddress().toString() 82\n"<<socket->peerAddress().toString();
            break;
        }
    }
}
