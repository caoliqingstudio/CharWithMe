#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    init();
}

void Login::init(){
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    //this->layout()->setSizeConstraint
    pwRetrievalWindows=NULL;
    registerWindows=NULL;
    ui->ipset->hide();
    ui->ipset->setText("127.0.0.1");//默认ip和端口
    ui->ipset->setPlaceholderText("ip");
    ui->portset->hide();
    ui->portset->setText("4321");
    ui->portset->setPlaceholderText("port");
    ui->pushButton_4->hide();
    ip=new QString;
    port=new QString;
    *ip=ui->ipset->text();
    *port=ui->portset->text();
}

Login::~Login()
{
    if(pwRetrievalWindows){
        delete pwRetrievalWindows;
    }
    if(registerWindows){
        delete registerWindows;
    }
    delete ui;
}

//注册
void Login::on_pushButton_clicked()
{
    registerWindows=new Register(this);
    registerWindows->ip=ip;
    registerWindows->port=port;
    registerWindows->show();
    this->hide();
    connect(registerWindows,SIGNAL(destroyed(QObject*)),this,SLOT(show()));
}

//找回密码
void Login::on_pushButton_3_clicked()
{
    pwRetrievalWindows=new PwRetrieval(this);
    pwRetrievalWindows->ip=ip;
    pwRetrievalWindows->port=port;
    pwRetrievalWindows->show();
    this->hide();
    connect(pwRetrievalWindows,SIGNAL(destroyed(QObject*)),this,SLOT(show()));
}

//登陆按钮
void Login::on_pushButton_2_clicked()
{
    QString username=ui->lineEdit->text();
    QString password=ui->lineEdit_2->text();
    MySocket con(*ip,*port);
    int state;
    if((state=con.sendLogin(username,password))!=SUCCESS){
        if(state==WRONG_ANSWER){
            QMessageBox::warning(this,tr("ByCLQ"),tr("Please change the username!"));
        }else{
            QMessageBox::warning(this,tr("ByCLQ"),tr("Please check the network!"));
        }
        return;
    }
    MainWindow *w=new MainWindow;
    w->ip=ip;
    w->port=port;
    w->username=username;
    w->init();
    w->show();
    connect(this,SIGNAL(destroyed(QObject*)),this,SLOT(deleteLater()));
    this->close();
}

void Login::on_findpwdButton_clicked()
{
    ui->ipset->show();
    ui->portset->show();
    ui->pushButton_4->show();
}

void Login::on_pushButton_4_clicked()
{
    ui->ipset->hide();
    ui->portset->hide();
    ui->pushButton_4->hide();
    *ip=ui->ipset->text();
    *port=ui->portset->text();
}
