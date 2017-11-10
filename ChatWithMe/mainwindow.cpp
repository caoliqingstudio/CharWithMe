#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::init(){
    QStringList usernames,informations,times;
    //Server *server=new Server;
    //server->init((*port).toInt());
    ui->label->setText(username);
    AddFriendWindows=NULL;
    mysocket=new MySocket(*ip,*port);
    friendList.clear();nicknameList.clear();timeFriendList.clear();
    ui->listWidget->clear();
    mysocket->sendConnect(username,&usernames,&informations,&times,&friendList,&nicknameList,&timeFriendList);//得到的消息要处理
    for(int i=0;i<friendList.count();i++){
        ui->listWidget->addItem(friendList.at(i)+"    "+timeFriendList.at(i)+"    "+nicknameList.at(i));//有点小小的命名问题 时间和备注反了
    }
    //消息处理
    for(int i=0;i<usernames.count();i++){
        qDebug()<<usernames.at(i)<<informations.at(i)<<timeFriendList.at(i);
        if(chatUserUI.contains(usernames.at(i))){
            chatUserUI.value(usernames.at(i))->addInfor(usernames.at(i),informations.at(i),times.at(i));
        }else{
            QString aimusername=usernames.at(i);
            QString aimuserip;
            if(mysocket->sendServer(usernames.at(i),&aimuserip)==SUCCESS){
                qDebug()<<"ip = "<<aimuserip;
                MySocket *friendSocket=new MySocket(aimuserip,QString::number(AIM_PORT));
                if(friendSocket->sendConnect()==SUCCESS){
                    //新建对象啊 没有对象怎么办 对象怎么写呢
                    qDebug()<<"找到对象了";
                    Chat * chatwindow=new Chat(this);
                    chatwindow->aimusername=aimusername;
                    chatwindow->username=username;
                    chatwindow->mysocket=friendSocket;
                    chatwindow->hostip=ip;chatwindow->hostport=port;
                    chatwindow->aimuserip=aimuserip;chatwindow->aimuserport=QString::number(AIM_PORT);
                    chatwindow->statehost=false;
                    chatwindow->init();
                    charUserIP.insert(aimusername,aimuserip);
                    chatUserUI.insert(aimusername,chatwindow);
                    connect(chatwindow,SIGNAL(destroyed(QObject*)),this,SLOT(deleteChat()));
                    chatwindow->show();
                    chatUserUI.value(usernames.at(i))->addInfor(usernames.at(i),informations.at(i),times.at(i));
                    continue;
                }else{
                    delete friendSocket;
                }
            }
            Chat * chatwindow=new Chat;
            chatwindow->aimusername=aimusername;
            chatwindow->username=username;
            chatwindow->mysocket=mysocket;
            chatwindow->hostip=ip;chatwindow->hostport=port;
            chatwindow->aimuserip=aimuserip;chatwindow->aimuserport=QString::number(AIM_PORT);
            chatwindow->statehost=true;
            chatwindow->init();
            charUserIP.insert(aimusername,*ip);
            chatUserUI.insert(aimusername,chatwindow);
            connect(chatwindow,SIGNAL(destroyed(QObject*)),this,SLOT(deleteChat()));
            //对象不在，给老大吧
            chatwindow->show();
            chatUserUI.value(usernames.at(i))->addInfor(usernames.at(i),informations.at(i),times.at(i));
        }
    }
}

MainWindow::~MainWindow()
{
    if(AddFriendWindows){
        AddFriendWindows->close();
        delete AddFriendWindows;
    }
    if(mysocket){
        delete mysocket;
    }
    delete ui;
}

//添加好友
void MainWindow::on_pushButton_clicked()
{
    if(!AddFriendWindows){
        AddFriendWindows=new AddFriend(this);
        AddFriendWindows->port=port;
        AddFriendWindows->ip=ip;
        AddFriendWindows->username=&username;
        AddFriendWindows->mysocket=mysocket;
    }
    //this->setEnabled(false);
    AddFriendWindows->show();
    connect(AddFriendWindows,SIGNAL(destroyed(QObject*)),this,SLOT(addItemFriend()));
}

void MainWindow::addItemFriend(){
    QString friendstr=AddFriendWindows->friendname+"    "+AddFriendWindows->nickname+"    now";
    friendList.append(friendstr);
    //ui->listWidget->clear();
    ui->listWidget->addItem(friendstr);
}

//双击 聊天

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString aimusername;
    QString untname=item->text();
    aimusername=untname.split("    ").at(0);
    QString aimuserip;
    qDebug()<<aimusername;
    if(mysocket->sendServer(aimusername,&aimuserip)==SUCCESS){
        qDebug()<<"ip = "<<aimuserip;
        MySocket *friendSocket=new MySocket(aimuserip,QString::number(AIM_PORT));
        if(friendSocket->sendConnect()==SUCCESS){
            //新建对象啊 没有对象怎么办 对象怎么写呢
            qDebug()<<"找到对象了";
            Chat * chatwindow=new Chat(this);
            chatwindow->aimusername=aimusername;
            chatwindow->username=username;
            chatwindow->mysocket=friendSocket;
            chatwindow->hostip=ip;chatwindow->hostport=port;
            chatwindow->aimuserip=aimuserip;chatwindow->aimuserport=QString::number(AIM_PORT);
            chatwindow->statehost=false;
            chatwindow->init();
            charUserIP.insert(aimusername,aimuserip);
            chatUserUI.insert(aimusername,chatwindow);
            connect(chatwindow,SIGNAL(destroyed(QObject*)),this,SLOT(deleteChat()));
            chatwindow->show();
            return;
        }else{
            delete friendSocket;
        }
    }
    Chat * chatwindow=new Chat;
    chatwindow->aimusername=aimusername;
    chatwindow->username=username;
    chatwindow->mysocket=mysocket;
    chatwindow->hostip=ip;chatwindow->hostport=port;
    chatwindow->aimuserip=aimuserip;chatwindow->aimuserport=QString::number(AIM_PORT);
    chatwindow->statehost=true;
    chatwindow->init();
    charUserIP.insert(aimusername,*ip);
    chatUserUI.insert(aimusername,chatwindow);
    connect(chatwindow,SIGNAL(destroyed(QObject*)),this,SLOT(deleteChat()));
    //对象不在，给老大吧
    chatwindow->show();
}


void MainWindow::deleteChat(){
    Chat * thischar = static_cast<Chat *>(sender());
    charUserIP.remove(thischar->aimusername);
    chatUserUI.remove(thischar->aimusername);
    disconnect(thischar,SIGNAL(destroyed(QObject*)),this,SLOT(deleteChat()));
    delete thischar;
}
