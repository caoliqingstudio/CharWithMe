#include "pwretrieval.h"
#include "ui_pwretrieval.h"

PwRetrieval::PwRetrieval(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PwRetrieval)
{
    ui->setupUi(this);
    init();
}

PwRetrieval::~PwRetrieval()
{
    delete ui;
}

void PwRetrieval::init(){
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_2->setPlaceholderText("<=16 characters");
}

//返回 登陆界面
void PwRetrieval::on_pushButton_2_clicked()
{
    this->destroyed();
    this->destroy();
}

//找回密码
void PwRetrieval::on_pushButton_clicked()
{
    QString username=ui->lineEdit_5->text();
    QString password=ui->lineEdit_2->text();
    QString question1=ui->label->text();
    QString answer1=ui->lineEdit_3->text();
    QString question2=ui->label_10->text();
    QString answer2=ui->lineEdit_4->text();
    QString question3=ui->label_11->text();
    QString answer3=ui->lineEdit_6->text();
    MySocket con(*ip,*port);
    int state;
    if((state=con.sendPwRetrieval(username,password,question1,answer1,question2,answer2,question3,answer3))!=SUCCESS){
        if(state==WRONG_ANSWER){
            QMessageBox::warning(this,tr("ByCLQ"),tr("Wrong answers!"));
        }else{
            QMessageBox::warning(this,tr("ByCLQ"),tr("Please check the network!"));
        }
        return;
    }
    this->hide();
    on_pushButton_2_clicked();
    QMessageBox::information(this,tr("密码修改成功"),username);
    return;
}

//输入完用户名
void PwRetrieval::on_lineEdit_5_editingFinished()
{
    MySocket TcpConnect(*ip,*port);
    QStringList question;
    if((TcpConnect.sendPwRetrievalAsk(ui->lineEdit_5->text(),&question))!=SUCCESS){
        QMessageBox::warning(this,tr("ByCLQ"),tr("Please check the network or username!"));
        return;
    }
    ui->label->setText(question.at(0));
    ui->label_10->setText(question.at(1));
    ui->label_11->setText(question.at(2));
}
