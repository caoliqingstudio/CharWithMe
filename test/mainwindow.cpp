#include "mainwindow.h"
#include "server.h"
#include "thread.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString portstr=ui->lineEdit->text();
    Server * server=new Server;
    server->init(portstr.toInt());
    ui->pushButton_2->setEnabled(false);
}

//发文件
void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton->setEnabled(false);
    const QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        Thread * thread=new Thread("127.0.0.1",fileName,"username","aimusername");
        thread->fileSR(ui->lineEdit->text().toInt());
    }
}
