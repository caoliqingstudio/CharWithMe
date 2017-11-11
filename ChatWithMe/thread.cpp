#include "thread.h"

Thread::Thread(QString ip, QString filename, QString username, QString aimusername){
    sendtype=true;
    this->ip=new QString(ip);
    this->filename=new QString(filename);
    this->username=new QString(username);
    this->aimusername=new QString(aimusername);
}

Thread::Thread(QTcpSocket *socket,long long filesize,QString filename, QString username){
    sendtype=false;
    mysocket=socket;
    this->filesize=filesize;
    this->ip=new QString("ip");
    this->filename=new QString(filename);
    this->username=new QString(username);
    this->aimusername=new QString("me");
}

Thread::~Thread(){
    delete filename;
    delete username;
    delete aimusername;
    delete ip;
}

void Thread::run(){
    if(sendtype){//发文件
        QTcpSocket sendsocket;
        QHostAddress address(ip);
        sendsocket.connectToHost(address,AIM_PORT);
        if(!sendsocket.waitForConnected(3000)){
            QMessageBox::warning(this,"SendFile","fail to connect!");
            this->deleteLater();
            return;
        }else{
            QString strtime;
            QDateTime time;

            time = QDateTime::currentDateTime();

            strtime = time.toString("yyyy-MM-dd hh:mm:ss");
            int filesize;
            QFile readfile(filename);
            if(!readfile.open(QFile::ReadOnly)){
                QMessageBox::warning(this,"SendFile","fail to open"+filename);
                this->deleteLater();
                return;
            }
            filesize=readfile.size();
            QString sendtext=QString::number(SENDFILE)+"\n"+username+"\n"
                    +aimusername+"\n"+filename+"\n"+QString::number(filesize)+"\n"+strtime+"\n";
            sendsocket.write(sendtext.toStdString().c_str());
            sendsocket.waitForBytesWritten();
            if(!sendsocket.waitForReadyRead()){
                QMessageBox::warning(this,"SendFile","fail to send;please check the net!");
                this->deleteLater();
                return;
            }
            std::string result=myconnect.readAll().toStdString();
            if(result.at(0)!=('0'+SENDFILE)){
                QMessageBox::warning(this,"SendFile","fail to send;please check the user!");
                this->deleteLater();
                return;
            }
            if(result.at(2)!=('0'+TRUE_REQUEST)){
                QMessageBox::warning(this,"SendFile","fail to send;please check the user!");
                this->deleteLater();
                return;
            } QDialog filewindow;
            filewindow.setWindowTitle(*aimusername+" "+*filename);
            QLabel lable1,lable2;
            lable1.setText("send:");
            QHBoxLayout layout;
            layout.addWidget(&lable1);
            layout.addWidget(&lable2);
            filewindow.setLayout(&layout);
            filewindow.show();
            //开始发送文件
        }
    }else{//收文件
        char result[]="0\n0\n";
        result[0]+=SENDFILE;
        result[2]+=TRUE_REQUEST;
        mysocket->readAll();//清空缓存
        mysocket->write(result);
        mysocket->waitForBytesWritten();
        QDialog filewindow;
        filewindow.setWindowTitle(*username+" "+*filename);
        QLabel lable1,lable2;
        lable1.setText("receive:");
        QHBoxLayout layout;
        layout.addWidget(&lable1);
        layout.addWidget(&lable2);
        filewindow.setLayout(&layout);
        filewindow.show();
        //开始收文件
    }
    this->deleteLater();
}
