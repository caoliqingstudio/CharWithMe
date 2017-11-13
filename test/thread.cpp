#include "thread.h"

Thread::Thread(QString ip, QString filename, QString username, QString aimusername){
    sendtype=true;
    text=new char[1056];
    this->ip=new QString(ip);
    this->filename=new QString(filename);
    this->username=new QString(username);
    this->aimusername=new QString(aimusername);
}

Thread::Thread(QTcpSocket *socket, quint64 filesize, QString filename, QString username, QString aimusername){
    sendtype=false;
    text=new char[1056];
    mysocket=socket;
    this->receivefilesize=filesize;
    this->ip=new QString("ip");
    this->filename=new QString(filename);
    this->username=new QString(username);
    this->aimusername=new QString(aimusername);
}

Thread::~Thread(){
    if(text){
        delete []text;
        delete filename;
        delete username;
        delete aimusername;
        delete ip;
    }
    text=0;
}

void Thread::fileSR(int port){
   // MySPNPlus myspn;
    if(sendtype){//发文件
        //myspn.getKey(*aimusername,1);
        QTcpSocket sendsocket;
        QHostAddress address(*ip);
        sendsocket.connectToHost(address,port);
        if(!sendsocket.waitForConnected(3000)){
            qDebug()<<"sendfile failt to connect";
            QMessageBox::warning(0,"SendFile","fail to connect!");
            //this->~Thread();//this->deleteLater();
            this->quit();return;
        }else{
            QString strtime;
            QDateTime time;

            time = QDateTime::currentDateTime();

            strtime = time.toString("yyyy-MM-dd hh:mm:ss");
            qint64 filesize;
            QFile readfile(*filename);
            QFile testfile(*filename+"__temp");testfile.open(QFile::WriteOnly);//测试
            if(!readfile.open(QFile::ReadOnly)){
                QMessageBox::warning(0,"SendFile","fail to open"+*filename);
                //this->~Thread();//this->deleteLater();
                this->quit();return;
            }
            QStringList fileList=filename->split('/');
            *filename=fileList.at(fileList.count()-1);
            qDebug()<<"will send file = "<<*filename;
            filesize=readfile.size();
            QString sendtext=QString::number(SENDFILE)+"\n"+username+"\n"
                    +aimusername+"\n"+*filename+"\n"+QString::number(filesize)+"\n"+strtime+"\n";
            qDebug()<<"\nfirst send to aim= "<<sendtext<<"\n\n";
            sendsocket.write(sendtext.toStdString().c_str());
            sendsocket.waitForBytesWritten();
            if(!sendsocket.waitForReadyRead()){
                QMessageBox::warning(0,"SendFile","fail to send;please check the net!");
                //this->~Thread();//this->deleteLater();
                this->quit();return;
            }
            std::string result=sendsocket.readAll().toStdString();
            if(result.at(0)!=('0'+SENDFILE)){
                QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
                readfile.close();
                //this->~Thread();//this->deleteLater();
                this->quit();return;
            }
            if(result.at(2)!=('0'+TRUE_REQUEST)){
                QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
                readfile.close();
                //this->~Thread();//this->deleteLater();
                this->quit();return;
            }
//            QDialog filewindow;
//            filewindow.setWindowTitle(*aimusername+" "+*filename);
//            filewindow.setMinimumHeight(100);
//            filewindow.setMinimumWidth(200);
//            QLabel lable1,lable2;
//            lable1.setText("send:");
//            lable2.setText("0%");
//            QHBoxLayout layout;
//            layout.addWidget(&lable1);
//            layout.addWidget(&lable2);
//            filewindow.setLayout(&layout);
//            filewindow.show();
            //开始发送文件
            //前4个字节是块序号
            //后四个字节是块大小
            //后8个字节是块验证
            quint32 num;
            quint32 first=1;
            quint64 tempend=0;
            quint64 havesend=0;
            //QMessageBox::warning(0,"send","start to send");
            while(num=readfile.read(text,1024)){
                quint32 endnum=8;
                char charstate;
                do{
                    sendsocket.write("send\n");
                    sendsocket.write((QString::number(first).toStdString()+'\n').c_str());
                    int numberwr=sendsocket.write(text,num);
                    //sendsocket.write(text,couts*8+16);
                    qDebug()<<"this time"<< first<<" file "<<num<<" has send"<<numberwr;
                    sendsocket.waitForBytesWritten();
                    if(sendsocket.waitForReadyRead()&&(endnum--)){
                        charstate=sendsocket.readAll().toStdString().at(0);
                        if(charstate=='1')qDebug()<<QString("fasongshibai\n");
                    }else{
                        qDebug()<<"lian xu fasong shibai hou\n";
                        //QMessageBox::warning(0,"SendFile","fail to send;please check the send!");
                        readfile.close();
                        //this->~Thread();//this->deleteLater();
                        this->quit();return;
                    }
                }while(charstate=='1');
                havesend+=num;
                first++;
                //lable2.setText(QString::number(havesend*100/filesize)+"%");
            }
            sendsocket.readAll();
            sendsocket.write("ovo\n");
            sendsocket.waitForBytesWritten();
            if(sendsocket.waitForReadyRead()){
                char charstate=sendsocket.readAll().toStdString().at(0);
                if(charstate!='A'){
                    QMessageBox::warning(0,QString("SendFile"),QString("send fail!"));
                }else{
                    QMessageBox::warning(0,QString("SendFile"),QString("send success!"));
                }
            }else{
                QMessageBox::warning(0,QString("SendFile"),"fail to send;please check the send!");
                readfile.close();
                //this->~Thread();//this->deleteLater();
                this->quit();return;
            }
        }
    }else{//收文件
        //myspn.getKey(*aimusername,-1);
        char result[]="0\n0\n";
        result[0]+=SENDFILE;
        result[2]+=TRUE_REQUEST;
        mysocket->readAll();//清空缓存
        mysocket->write(result);
        mysocket->waitForBytesWritten();
//        QDialog filewindow;
//        filewindow.setMinimumHeight(100);
//        filewindow.setMinimumWidth(200);
//        filewindow.setWindowTitle(*username+" "+*filename);
//        QLabel lable1,lable2;
//        lable1.setText("receive:");
//        QHBoxLayout layout;
//        layout.addWidget(&lable1);
//        layout.addWidget(&lable2);
//        filewindow.setLayout(&layout);
//        filewindow.show();
        QFile receivefile(*filename);
        if(!receivefile.open(QFile::WriteOnly)){
            QMessageBox::warning(0,"ReceiveFile",QString("fail to create")+filename);
            receivefile.close();
            //this->~Thread();//this->deleteLater();
            this->quit();return;
        }
        //开始收文件
        quint32 first=1;
        quint64 tempend=0;
        quint64 havereceive=0;
        QString strtime;
        QDateTime time;

        time = QDateTime::currentDateTime();

        strtime = time.toString("yyyy-MM-dd hh:mm:ss");
        qDebug()<<"start to receive fiel "<<strtime;
        while(mysocket->waitForReadyRead()){
            QString receivetxt=QString::fromStdString(mysocket->readLine().toStdString());
            QString receivefirst=QString::fromStdString(mysocket->readLine().toStdString());receivefirst.chop(1);
            if(receivetxt.compare("send\n")){
//                quint64 receiveend;
//                mysocket->read((char *)&blockfirst,4);
//                mysocket->read((char *)&receiveend,8);
//                if(receiveend!=tempend){
//                    QMessageBox::warning(0,"ReceiveFile","fail to receive;Wrong file!");
//                    receivefile.close();
//                    //this->~Thread();//this->deleteLater();
//                    this->quit();return;
//                }
                mysocket->write("A\n");
                mysocket->waitForBytesWritten();
                break;
            }else if(receivefirst.toInt()!=first){
                mysocket->readAll();
                qDebug()<<"fail to receive the block = "<<receivefirst;
                mysocket->write("1\n");
                mysocket->waitForBytesWritten();
            }
            QByteArray bytearry= mysocket->readAll();
            char *receive=bytearry.data();
            int blocknumR=bytearry.size();
//            int num=blockNum/8-1;
//            for(int i=0;i<num;i++){
//                temp=temp^*(((quint64*)text)+i);
//                //(*(((quint64*)text)+i))=myspn.decrypt(*(((quint64*)text)+i));
//            }
//            temp=temp^*(((unsigned long long*)text)+num);
//            //(*(((unsigned long long*)text)+num))=myspn.decrypt(*(((unsigned long long*)text)+num));
//            int j=7;
//            while(text[num*8+j]==0){
//                j--;
//                blockNum--;
//            }
//            if(temp!=receivetemp){
//                mysocket->readAll();
//                mysocket->write("1\n");
//                mysocket->waitForBytesWritten();
//                continue;
//            }
            quint64 blockNum=receivefile.write(bytearry);
            qDebug()<<"have receive "<<first++<<" receive"<<receivefirst<<" receice "<<blocknumR<<" file "<<blockNum<<" blocksize "<<strlen(receive);
            //tempend=tempend^temp;
            //first++;
            havereceive+=blockNum;
            //lable2.setText(QString::number(havereceive*100/receivefilesize)+"%");
            mysocket->readAll();
            mysocket->write("0\n");
            mysocket->waitForBytesWritten();
            //sleep(10);
        }

        time = QDateTime::currentDateTime();

        strtime = time.toString("yyyy-MM-dd hh:mm:ss");
        qDebug()<<"end to receive fiel "<<strtime;
        receivefile.close();
    }
    //this->~Thread();//this->deleteLater();
}

void Thread::run(){
//    MySPNPlus myspn;
//    if(sendtype){//发文件
//        myspn.getKey(*aimusername,1);
//        QTcpSocket sendsocket;
//        QHostAddress address(*ip);
//        sendsocket.connectToHost(address,AIM_PORT);
//        if(!sendsocket.waitForConnected(3000)){
//            qDebug()<<"sendfile failt to connect";
//            QMessageBox::warning(0,"SendFile","fail to connect!");
//            //this->~Thread();//this->deleteLater();
//            this->quit();return;
//        }else{
//            QString strtime;
//            QDateTime time;

//            time = QDateTime::currentDateTime();

//            strtime = time.toString("yyyy-MM-dd hh:mm:ss");
//            qint64 filesize;
//            QFile readfile(*filename);
//            //QFile testfile(*filename+"__temp");testfile.open(QFile::WriteOnly);//测试
//            if(!readfile.open(QFile::ReadOnly)){
//                QMessageBox::warning(0,"SendFile","fail to open"+*filename);
//                //this->~Thread();//this->deleteLater();
//                this->quit();return;
//            }
//            QStringList fileList=filename->split('/');
//            *filename=fileList.at(fileList.count()-1);
//            qDebug()<<"will send file = "<<*filename;
//            filesize=readfile.size();
//            QString sendtext=QString::number(SENDFILE)+"\n"+username+"\n"
//                    +aimusername+"\n"+*filename+"\n"+QString::number(filesize)+"\n"+strtime+"\n";
//            qDebug()<<"\nfirst send to aim= "<<sendtext<<"\n\n";
//            sendsocket.write(sendtext.toStdString().c_str());
//            sendsocket.waitForBytesWritten();
//            if(!sendsocket.waitForReadyRead()){
//                QMessageBox::warning(0,"SendFile","fail to send;please check the net!");
//                //this->~Thread();//this->deleteLater();
//                this->quit();return;
//            }
//            std::string result=sendsocket.readAll().toStdString();
//            if(result.at(0)!=('0'+SENDFILE)){
//                QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
//                readfile.close();
//                //this->~Thread();//this->deleteLater();
//                this->quit();return;
//            }
//            if(result.at(2)!=('0'+TRUE_REQUEST)){
//                QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
//                readfile.close();
//                //this->~Thread();//this->deleteLater();
//                this->quit();return;
//            }
//            QDialog filewindow;
//            filewindow.setWindowTitle(*aimusername+" "+*filename);
//            filewindow.setMinimumHeight(100);
//            filewindow.setMinimumWidth(200);
//            QLabel lable1,lable2;
//            lable1.setText("send:");
//            lable2.setText("0%");
//            QHBoxLayout layout;
//            layout.addWidget(&lable1);
//            layout.addWidget(&lable2);
//            filewindow.setLayout(&layout);
//            filewindow.show();
//            //开始发送文件
//            //前4个字节是块序号
//            //后四个字节是块大小
//            //后8个字节是块验证
//            quint32 num;
//            quint32 first=1;
//            quint64 tempend=0;
//            quint64 havesend=0;
//            while(num=readfile.read(text+16,1024)){
//                quint64 temp=0;
//                quint32 couts=num/8;
//                if(num!=1024){
//                    if((num%8)){
//                       couts++;
//                       text[num+16]=0;text[num+17]=0;text[num+18]=0;text[num+19]=0;
//                       text[num+20]=0;text[num+21]=0;text[num+22]=0;text[num+23]=0;
//                    }
//                }
//                for(quint32 i=0;i<num;i++){
//                    *(((quint64*)(text+16))+i)=myspn.encrypt(*(((quint64*)(text+16))+i));
//                    temp=temp^*(((quint64 *)(text+16))+i);
//                }
//                tempend=tempend^temp;
//                *(quint32*)text=first;
//                *(((quint32 *)text)+1)=couts*8;
//                *(((quint64 *)text)+1)=temp;
//                char charstate;
//                int endnum=8;
////                testfile.write(text,couts*8+16);
//                do{
//                    sendsocket.write(text,couts*8+16);
//                    sendsocket.waitForBytesWritten();
//                    if(sendsocket.waitForReadyRead()&&(endnum--)){
//                        charstate=sendsocket.readAll().toStdString().at(0);
//                        //charstate='0';//test
//                        if(charstate=='1')qDebug()<<QString("fasongshibai\n");
//                    }else{
//                        qDebug()<<"lian xu fasong shibai hou\n";
//                        //QMessageBox::warning(0,"SendFile","fail to send;please check the send!");
//                        readfile.close();
//                        //this->~Thread();//this->deleteLater();
//                        this->quit();return;
//                    }
//                }while(charstate=='1');
//                havesend+=num;
//                first++;
//                //lable2.setText(QString::number(havesend*100/filesize)+"%");
//            }
//            *(quint32*)text=0;
//            *(((quint32 *)text)+1)=0;
//            *(((quint64 *)text)+1)=tempend;
//            sendsocket.readAll();
//            sendsocket.write(text,16);
//            sendsocket.waitForBytesWritten();
//            if(sendsocket.waitForReadyRead()){
//                char charstate=sendsocket.readAll().toStdString().at(0);
//                if(charstate!='A'){
//                    QMessageBox::warning(0,QString("SendFile"),QString("send fail!"));
//                }else{
//                    QMessageBox::warning(0,QString("SendFile"),QString("send success!"));
//                }
//            }else{
//                QMessageBox::warning(0,QString("SendFile"),"fail to send;please check the send!");
//                readfile.close();
//                //this->~Thread();//this->deleteLater();
//                this->quit();return;
//            }
//        }
//    }else{//收文件
//        myspn.getKey(*aimusername,-1);
//        char result[]="0\n0\n";
//        result[0]+=SENDFILE;
//        result[2]+=TRUE_REQUEST;
//        mysocket->readAll();//清空缓存
//        mysocket->write(result);
//        mysocket->waitForBytesWritten();
//        QDialog filewindow;
//        filewindow.setMinimumHeight(100);
//        filewindow.setMinimumWidth(200);
//        filewindow.setWindowTitle(*username+" "+*filename);
//        QLabel lable1,lable2;
//        lable1.setText("receive:");
//        QHBoxLayout layout;
//        layout.addWidget(&lable1);
//        layout.addWidget(&lable2);
//        filewindow.setLayout(&layout);
//        filewindow.show();
//        QFile receivefile(*filename);
//        if(!receivefile.open(QFile::WriteOnly)){
//            QMessageBox::warning(0,"ReceiveFile",QString("fail to create")+filename);
//            receivefile.close();
//            //this->~Thread();//this->deleteLater();
//            this->quit();return;
//        }
//        //开始收文件
//        quint32 first=1;
//        quint64 tempend=0;
//        quint64 havereceive=0;
//        while(mysocket->waitForReadyRead()){
//            quint32 blockfirst;
//            mysocket->read((char *)&blockfirst,4);
//            if(!blockfirst){
//                quint64 receiveend;
//                mysocket->read((char *)&blockfirst,4);
//                mysocket->read((char *)&receiveend,8);
//                if(receiveend!=tempend){
//                    QMessageBox::warning(0,"ReceiveFile","fail to receive;Wrong file!");
//                    receivefile.close();
//                    //this->~Thread();//this->deleteLater();
//                    this->quit();return;
//                }
//                mysocket->write("A\n");
//                mysocket->waitForBytesWritten();
//                break;
//            }else if(blockfirst!=first){
//                QMessageBox::warning(0,"ReceiveFile","fail to receive;please check the send!");
//                receivefile.close();
//                //this->~Thread();//this->deleteLater();
//                this->quit();return;
//            }
//            quint32 blockNum;
//            mysocket->read((char *)&blockNum,4);
//            quint64 receivetemp,temp=0;
//            mysocket->read(((char*)&receivetemp),8);
//            //处理 唉唉唉唉啊啊啊  图书馆 下班了 坑 坑 坑
//            mysocket->read(text,blockNum);
//            int num=blockNum/8-1;
//            for(int i=0;i<num;i++){
//                temp=temp^*(((quint64*)text)+i);
//                (*(((quint64*)text)+i))=myspn.decrypt(*(((quint64*)text)+i));
//            }
//            temp=temp^*(((unsigned long long*)text)+num);
//            (*(((unsigned long long*)text)+num))=myspn.decrypt(*(((unsigned long long*)text)+num));
//            int j=7;
//            while(text[num*8+j]==0){
//                j--;
//                blockNum--;
//            }
//            if(temp!=receivetemp){
//                mysocket->readAll();
//                mysocket->write("1\n");
//                mysocket->waitForBytesWritten();
//                continue;
//            }
//            receivefile.write(text,blockNum);
//            tempend=tempend^temp;
//            first++;
//            havereceive+=blockNum;
//            lable2.setText(QString::number(havereceive*100/receivefilesize)+"%");
//            mysocket->readAll();
//            mysocket->write("0\n");
//            mysocket->waitForBytesWritten();
//        }
//        receivefile.close();
//    }
//    //this->~Thread();//this->deleteLater();
}
