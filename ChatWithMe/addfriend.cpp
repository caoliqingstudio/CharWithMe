#include "addfriend.h"
#include "ui_addfriend.h"

AddFriend::AddFriend(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);
}

AddFriend::~AddFriend()
{
    delete ui;
}

//添加好友
void AddFriend::on_pushButton_clicked()
{
    friendname=ui->lineEdit_5->text();
    nickname=ui->lineEdit->text();
    if(friendname.isEmpty()){
       QMessageBox::warning(this,"ByCLQ","Name is empty");
       return;
    }
    if(mysocket->sendAddFriend(*username,friendname,nickname)!=SUCCESS){
        QMessageBox::warning(this,"ByCLQ","Wrong friendname or bad host!");
    }else{
        QMessageBox::warning(this,"ByCLQ","Add "+friendname+" success!");
        this->hide();
        this->destroyed();
    }
}

//返回主界面
void AddFriend::on_pushButton_2_clicked()
{
    this->hide();
}
