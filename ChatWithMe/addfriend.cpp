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

}

//返回主界面
void AddFriend::on_pushButton_2_clicked()
{
    this->destroyed();
    this->destroy();
}
