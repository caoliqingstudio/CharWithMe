#include "chat.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    ui->listWidget->setLayoutMode(QListWidget::Batched);
    hostip=NULL;
    hostport=NULL;
    statehost=true;
    mysocket=NULL;
    socket=NULL;
}

void Chat::init(){
  if(socket){
      mysocket=new MySocket(aimuserip,aimuserport,socket);
  }
  ui->textEdit->document()->setMaximumBlockCount(200);
  ui->label_2->setText(aimusername);
  ui->pushButton_2->setEnabled(!statehost);
}

void Chat::init(MySocket *socket){
    init();
    statehost=false;
    mysocket=socket;
}

Chat::~Chat()
{
    delete ui;
}

void Chat::on_pushButton_clicked()
{
    QString text=ui->textEdit->toPlainText();
    if(SUCCESS==mysocket->sendText(SENDTEXT,text,username,aimusername)){
        QString strtime;
        QDateTime time;

        time = QDateTime::currentDateTime();

        strtime = time.toString("yyyy-MM-dd hh:mm:ss");
        addInfor(username,text,strtime);
        ui->textEdit->clear();
    }else{
        QMessageBox::warning(this,"HOSTERROR","please check the net");
    }
}

void Chat::addInfor(QString name, QString infor, QString time){
    ui->listWidget->addItem(name+"  "+time+":\n"+infor);
    ui->listWidget->scrollToBottom();
}

void Chat::on_pushButton_2_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        QString strtime;
        QDateTime time;

        time = QDateTime::currentDateTime();
        MySocket fileSocket(mysocket->myconnect->peerAddress().toString(),QString::number(AIM_PORT));
        strtime = time.toString("yyyy-MM-dd hh:mm:ss");
        addInfor(username,"send "+fileName+"start!",strtime);
        if(fileSocket.sendFile(fileName,username,aimusername)==SUCCESS){
            time = QDateTime::currentDateTime();
            strtime = time.toString("yyyy-MM-dd hh:mm:ss");
            addInfor(username,"send "+fileName+" end!",strtime);
        }else{
            time = QDateTime::currentDateTime();
            strtime = time.toString("yyyy-MM-dd hh:mm:ss");
            addInfor(username,"send "+fileName+"fail!",strtime);
        }
    }
}

