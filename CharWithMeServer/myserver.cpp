#include "myserver.h"


Server::Server(QObject *parent) : QObject(parent)
{
    stateQmapClient=true;
    stateQmapThread=true;
    m_tcpServer = new QTcpServer();
    m_tcpServer->setMaxPendingConnections(2000);
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectSlot()));
}

Server::~Server()
{

}

void Server::init(int port)
{
    if(m_tcpServer->listen(QHostAddress::Any, port)){
        qDebug() << "listen OK!";
    }else{
        qDebug() << "listen error!";
    }
}

//void Server::sendData(QString ip, QString data)
//{
//    m_mapClient.value(ip)->write(data.toLatin1());
//}
//void Server::sendData(QString ip, uchar *rawData)
//{
//    m_mapClient.value(ip)->write(QByteArray((char*)rawData));
//}

void Server::newConnectSlot()
{
    QTcpSocket *tcp = m_tcpServer->nextPendingConnection();
    QThread *thread = new QThread(this);
    connect(tcp,SIGNAL(readyRead()),this,SLOT(readMessage()));
    thread->start();
    tcp->moveToThread(thread);
//    QTcpSocket *tcp = m_tcpServer->nextPendingConnection();
//    Thread *nextThread=new Thread;
//    nextThread->ui=ui;
//    nextThread->myserver=this;
//    nextThread->type=0;
//    nextThread->myconnect=tcp;
//    nextThread->start();
    m_mapClient.insert(tcp->peerAddress().toString(), tcp);
    //m_mapThread.insert(tcp->peerAddress().toString(),nextThread);
    //qDebug()<<"\n  this is tcp->peerAddress 52 \n"<<tcp->peerAddress();

    connect(tcp,SIGNAL(disconnected()),this,SLOT(removeUserFormList()));
}

void Server::readMessage()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    //qDebug()<<"  this is socket->peerAddress 66\n"<<socket->peerAddress();
    std::string result=socket->readLine().toStdString();
    switch (result.at(0)) {
    case ('0'+CONNECT):{
        QString username=QString::fromStdString(socket->readLine().toStdString());username.chop(1);
        char result[]="0\n0\n";
        result[0]+=CONNECT;
        QStringList usernames,infomations,times;
        result[2]+=TRUE_REQUEST;
        QStringList friends,nicknames,friendtimes;
        db.findFriends(username,&friends,&nicknames,&friendtimes);
        bool state=db.findOffLine(username,&usernames,&infomations,&times);
        QString resultQSt=QString(result)+QString::number(friends.count())+"\n"+QString::number(usernames.count())+"\n";
        for(int i=0;i<friends.count();i++){
            resultQSt+= friends.at(i)+"\n"+nicknames.at(i)+"\n"+friendtimes.at(i)+"\n";
        }
        resultQSt+="friendover\n";
//        qDebug()<<"first send"<<resultQSt;
//        socket->write(resultQSt.toStdString().c_str());
//        socket->waitForBytesWritten(3000);
        if(state){
            for(int i=0;i<usernames.count();i++) {
                resultQSt += usernames.at(i)+"\n"+infomations.at(i)+"\n"+times.at(i)+"\n";
            }
            db.deleteOffLine(username);
        }
        resultQSt += "over\n";
        qDebug()<<"first send"<<resultQSt;
        socket->write(resultQSt.toStdString().c_str());
        socket->waitForBytesWritten(3000);
        QString ip;
        if(!db.findOnline(username,&ip)){
            db.insertOnline(username,socket->peerAddress().toString());
        }else if(ip.compare(socket->peerAddress().toString())){
            db.deleteOnline(username);
            db.insertOnline(username,socket->peerAddress().toString());
        }
        qDebug()<<"connect insert into db"<<socket->peerAddress().toString();
        QStringList onlines;
        ui->label_4->setText(QString::number(db.onlineNumber(&onlines)));
        ui->listWidget_2->clear();
        ui->listWidget_2->addItems(onlines);
        qDebug()<<"connect flash ui";
    }
        break;
    case ('0'+SENDTEXT):{
        QString username=QString::fromStdString(socket->readLine().toStdString());
        username.chop(1);
        QString aimusername=QString::fromStdString(socket->readLine().toStdString());aimusername.chop(1);
        QString thistime=QString::fromStdString(socket->readLine().toStdString());thistime.chop(1);
        QString information=QString::fromStdString(socket->readAll().toStdString());information.chop(1);
        db.insertOffLine(username,aimusername,information,thistime);
    }
        break;
    case ('0'+LOGIN):{
        QString username=QString::fromStdString(socket->readLine().toStdString());username.chop(1);
        QString password=QString::fromStdString(socket->readLine().toStdString());password.chop(1);
        if(db.findUser(username,&password)){
            char result[]="0\n0\n";
            result[0]+=LOGIN;
            result[2]+=TRUE_REQUEST;
            socket->write(result);
            socket->waitForBytesWritten();
            break;
        }
        char result[]="0\n0\n";
        result[0]+=LOGIN;
        result[2]+=FALSE_REQUEST;
        socket->write(result);
        socket->waitForBytesWritten();
    }
        break;
    case ('0'+REGISTER):{
        QString username=QString::fromStdString(socket->readLine().toStdString());username.chop(1);
        QString password=QString::fromStdString(socket->readLine().toStdString());password.chop(1);
        QString q1=QString::fromStdString(socket->readLine().toStdString());q1.chop(1);
        QString a1=QString::fromStdString(socket->readLine().toStdString());a1.chop(1);
        QString q2=QString::fromStdString(socket->readLine().toStdString());q2.chop(1);
        QString a2=QString::fromStdString(socket->readLine().toStdString());a2.chop(1);
        QString q3=QString::fromStdString(socket->readLine().toStdString());q3.chop(1);
        QString a3=QString::fromStdString(socket->readLine().toStdString());a3.chop(1);
        //qDebug()<<"\n pass word = "<<password;//
        if(!db.findUser(username)){
            if(db.insertUser(username,password,q1,a1,q2,a2,q3,a3)){
                char result[]="0\n0\n";
                result[0]+=REGISTER;
                result[2]+=TRUE_REQUEST;
                socket->write(result);
                socket->waitForBytesWritten();
                QStringList users;
                ui->label_2->setText(QString::number(db.userNumber(&users)));
                ui->listWidget->clear();
                ui->listWidget->addItems(users);
                break;
            }
        }
        char result[]="0\n0\n";
        result[0]+=REGISTER;
        result[2]+=FALSE_REQUEST;
        socket->write(result);
        socket->waitForBytesWritten();
    }
        break;
    case ('0'+PWRE):{
        QString username=QString::fromStdString(socket->readLine().toStdString());username.chop(1);
        QString password=QString::fromStdString(socket->readLine().toStdString());password.chop(1);
        QString q1=QString::fromStdString(socket->readLine().toStdString());q1.chop(1);
        QString a1=QString::fromStdString(socket->readLine().toStdString());a1.chop(1);
        QString q2=QString::fromStdString(socket->readLine().toStdString());q2.chop(1);
        QString a2=QString::fromStdString(socket->readLine().toStdString());a2.chop(1);
        QString q3=QString::fromStdString(socket->readLine().toStdString());q3.chop(1);
        QString a3=QString::fromStdString(socket->readLine().toStdString());a3.chop(1);
        //qDebug()<<"\n PWRE password = "<<password;//
        if(db.findPwRe(username,password,q1,a1,q2,a2,q3,a3)){
             char result[]="0\n0\n";
             result[0]+=PWRE;
             result[2]+=TRUE_REQUEST;
             QString resultQSt=QString(result)+q1+"\n"+q2+"\n"+q3+"\n";
             socket->write(resultQSt.toStdString().c_str());
             socket->waitForBytesWritten();
             break;
        }
        char result[]="0\n0\n";
        result[0]+=PWRE;
        result[2]+=FALSE_REQUEST;
        socket->write(result);
        socket->waitForBytesWritten();
    }

        break;
    case ('0'+ADD):{
        QString username=QString::fromStdString(socket->readLine().toStdString());username.chop(1);
        QString friendname=QString::fromStdString(socket->readLine().toStdString());friendname.chop(1);
        QString nickname=QString::fromStdString(socket->readLine().toStdString());nickname.chop(1);
        qDebug()<<"\n add "<<username<<friendname<<nickname;
        if(db.findUser(username)){
            QString strtime;
            QDateTime time;
            time = QDateTime::currentDateTime();
            strtime = time.toString("yyyy-MM-dd hh:mm:ss");
            if(db.insertFriends(username,friendname,strtime,nickname)&&db.insertFriends(friendname,username,strtime)){
                QString ip;
                char result[]="0\n0\n";
                result[0]+=ADD;
                result[2]+=TRUE_REQUEST;
                QString resultQSt=QString(result);
                socket->write(resultQSt.toStdString().c_str());
                socket->waitForBytesWritten(3000);
                if(db.findOnline(friendname,&ip)&&m_mapClient.contains(ip)){
                     int type=ADD;
                     QString string;
                     QString typeString=QString::number(type);
                     string=typeString+"\n"+username+"\n"+friendname+"\n";
                     QTcpSocket thisSocket(this);
                     QHostAddress address(ip);
                     thisSocket.connectToHost(address,AIM_PORT);
                     if(!thisSocket.waitForConnected(300)){
                         thisSocket.write(string.toStdString().c_str());
                         thisSocket.waitForBytesWritten(300);
                     }
                 }
                 break;
            }
        }
        char result[]="0\n0\n";
        result[0]+=ADD;
        result[2]+=FALSE_REQUEST;
        socket->write(result);
        socket->waitForBytesWritten(3000);
    }
        break;
    case ('0'+SENDFILE):

        break;
    case ('0'+LISTENING):

        break;
    case ('0'+PWRE_ASK):{
        QString username=QString::fromStdString(socket->readLine().toStdString());username.chop(1);
        QString q1,q2,q3;
        if(db.findUser(username)){
            if(db.findPwReAsk(username,&q1,&q2,&q3)){
                char result[]="0\n0\n";
                result[0]+=PWRE_ASK;
                result[2]+=TRUE_REQUEST;
                QString resultQSt=QString(result)+q1+"\n"+q2+"\n"+q3+"\n";
                qDebug()<<resultQSt;
                socket->write(resultQSt.toStdString().c_str());
                socket->waitForBytesWritten(3000);
                break;
            }
        }
        char result[]="0\n0\n";
        result[0]+=PWRE_ASK;
        result[2]+=FALSE_REQUEST;
        socket->write(result);
        socket->waitForBytesWritten(3000);
    }
        break;
    case ('0'+SERVER):{
        QString username=QString::fromStdString(socket->readLine().toStdString());username.chop(1);
        if(db.findUser(username)){
            QString ip;
            if(db.findOnline(username,&ip)){
                int type=SERVER;
                QString string;
                QString typeString=QString::number(type);
                string=typeString+"\n"+QString::number(TRUE_REQUEST)+"\n"+username+"\n"+ip+"\n";
                qDebug()<<"ni dui xiang ip "<<string;
                socket->write(string.toStdString().c_str());
                socket->waitForBytesWritten(3000);
            }
        }
        char result[]="0\n0\n";
        result[0]+=SERVER;
        result[2]+=FALSE_REQUEST;
        socket->write(result);
        socket->waitForBytesWritten(3000);
    }
        break;
    default:
        break;
    }
}

void Server::removeUserFormList()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    db.deleteOnline(socket->peerAddress().toString(),1);
    QStringList onlines;
    ui->label_4->setText(QString::number(db.onlineNumber(&onlines)));
    ui->listWidget_2->clear();
    ui->listWidget_2->addItems(onlines);
    for(QMap<QString, QTcpSocket *>::iterator it=m_mapClient.begin();it!=m_mapClient.end();it++)
    {
        if(socket->peerAddress().toString() == it.key())
        {
            m_mapClient.erase(it);
            //m_pMsgHandler->devOffline(socket->peerAddress().toString());
            qDebug()<<"this is socket->peerAddress().toString() 82\n"<<socket->peerAddress().toString();
            break;
        }
    }

    for(QMap<QString, void *>::iterator it=m_mapThread.begin();it!=m_mapThread.end();it++)
    {
        if(socket->peerAddress().toString() == it.key())
        {
            //delete (Thread *)m_mapThread.value(socket->peerAddress().toString());
            m_mapThread.erase(it);
            //m_pMsgHandler->devOffline(socket->peerAddress().toString());
            //qDebug()<<"this is socket->peerAddress().toString() 82\n"<<socket->peerAddress().toString();
            break;
        }
    }
}
