#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    init();
}

Register::~Register()
{
    delete ui;
}

void Register::init(){
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,this);
    ui->lineEdit->setValidator( validator );
    ui->lineEdit->setPlaceholderText("<=16 letters or numbers");
    ui->lineEdit_2->setPlaceholderText("<=16 characters");
}

//返回 登陆界面
void Register::on_pushButton_2_clicked()
{
    this->destroyed();
    this->destroy();
}

//注册
void Register::on_pushButton_clicked()
{
    QString username=ui->lineEdit->text();
    QString password=ui->lineEdit_2->text();
    QString question1=ui->comboBox->currentText();
    QString answer1=ui->lineEdit_3->text();
    QString question2=ui->comboBox_2->currentText();
    QString answer2=ui->lineEdit_4->text();
    QString question3=ui->comboBox_3->currentText();
    QString answer3=ui->lineEdit_5->text();
    if(question1==question2||question1==question3||question2==question3){
        QMessageBox::warning(this,tr("ByCLQ"),tr("Please choose diffrient question!"));
        return;
    }
    MySocket con(*ip,*port);
    int state;
    if((state=con.sendRegister(username,password,question1,answer1,question2,answer2,question3,answer3))!=SUCCESS){
        if(state==WRONG_ANSWER){
            QMessageBox::warning(this,tr("ByCLQ"),tr("Please change the username!"));
        }else{
            QMessageBox::warning(this,tr("ByCLQ"),tr("Please check the network!"));
        }
        return;
    }
    this->destroyed();
    this->destroy();
    QMessageBox::information(this,tr("注册成功"),username);
    on_pushButton_2_clicked();
    return;
}

//用户名输入完毕
void Register::on_lineEdit_editingFinished()
{
    qDebug()<<"do nothing";
}
