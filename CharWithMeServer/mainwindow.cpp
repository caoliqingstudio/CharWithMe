#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText("4321");
    db=new DataBase(1);
    QStringList users;
    ui->label_2->setText(QString::number(db->userNumber(&users)));
    ui->listWidget->clear();
    ui->listWidget->addItems(users);
    QStringList onlines;
    ui->label_4->setText(QString::number(db->onlineNumber(&onlines)));
    ui->listWidget_2->addItems(onlines);
}

MainWindow::~MainWindow()
{
    delete db;
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->setEnabled(false);
    port=ui->lineEdit->text();
    myserver=new Server;
    ((Server *)myserver)->ui=ui;
    ((Server *)myserver)->init(port.toInt());
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::on_pushButton_clicked()
{
    QString username=ui->listWidget->currentItem()->text();
    if(username.isEmpty()){
        QMessageBox::warning(this,"delete","please choose the user!");
        return;
    }
    db->deleteAll(username);
    QStringList users;
    ui->label_2->setText(QString::number(db->userNumber(&users)));
    ui->listWidget->clear();
    ui->listWidget->addItems(users);
    QStringList onlines;
    ui->label_4->setText(QString::number(db->onlineNumber(&onlines)));
    ui->listWidget_2->clear();
    ui->listWidget_2->addItems(onlines);
}
