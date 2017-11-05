#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::init(){

    Server *server=new Server;
    server->init((*port).toInt());
    AddFriendWindows=NULL;
    MySocket mysocket(*ip,*port);
    std::cout<<mysocket.sendText(SENDTEXT,tr("asdfasdf"));
}

MainWindow::~MainWindow()
{
    if(AddFriendWindows){
        AddFriendWindows->close();
        delete AddFriendWindows;
    }
    delete ui;
}

//添加好友
void MainWindow::on_pushButton_clicked()
{
    AddFriendWindows=new AddFriend(this);
    AddFriendWindows->port=port;
    AddFriendWindows->ip=ip;
    //this->setEnabled(false);
    AddFriendWindows->show();
    //connect(AddFriendWindows,SIGNAL(destroyed(QObject*)),this,SLOT(setEnabled(bool)));
}

//双击 聊天
void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{

}
