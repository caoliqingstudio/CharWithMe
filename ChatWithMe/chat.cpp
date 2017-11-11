#include "chat.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    hostip=NULL;
    hostport=NULL;
    statehost=true;
    mysocket=NULL;
    socket=NULL;
}

void Chat::init(){
  ui->textEdit->document()->setMaximumBlockCount(200);
  ui->label_2->setText(aimusername);
  ui->pushButton_2->setEnabled(!statehost);
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
}

void Chat::on_pushButton_2_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        Thread * thread=new Thread(0,aimuserip,filename,username,aimusername);
        thread->start();
    }
}
