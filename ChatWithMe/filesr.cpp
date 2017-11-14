#include "filesr.h"

FileSR::FileSR()
{

}

bool FileSR::fileSend(QTcpSocket *socket, QString filename, QString username, QString aimusername){
    socket->readAll();
    MySPNPlus myspn;
    myspn.getKey(aimusername,1);
    QString strtime;
    QDateTime time;
    time = QDateTime::currentDateTime();
    strtime = time.toString("yyyy-MM-dd hh:mm:ss");
    qint64 filesize;
    QFile readfile(filename);
    if(!readfile.open(QFile::ReadOnly)){
        QMessageBox::warning(0,"SendFile","fail to open"+filename);
        return false;
    }
    QStringList fileList=filename.split('/');
    filename=fileList.at(fileList.count()-1);
    qDebug()<<"will send file = "<<filename;
    filesize=readfile.size();
    QString sendtext=QString::number(SENDFILE)+"\n"+username+"\n"
          +aimusername+"\n"+filename+"\n"+QString::number(filesize)+"\n"+strtime+"\n";
    qDebug()<<"\nfirst send to aim= "<<sendtext<<"\n\n";
    socket->write(sendtext.toStdString().c_str());
    socket->waitForBytesWritten();
    if(!socket->waitForReadyRead()){
        QMessageBox::warning(0,"SendFile","fail to send;please check the net!");
        return false;
    }
    std::string result=socket->readAll().toStdString();
    if(result.at(0)!=('0'+SENDFILE)){
        QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
        readfile.close();
        return false;
    }
    if(result.at(2)!=('0'+TRUE_REQUEST)){
        QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
        readfile.close();
        return false;
    }
//    QDialog filewindow;
//    filewindow.setWindowTitle(*aimusername+" "+*filename);
//    filewindow.setMinimumHeight(100);
//    filewindow.setMinimumWidth(200);
//    QLabel lable1,lable2;
//    lable1.setText("send:");
//    lable2.setText("0%");
//    QHBoxLayout layout;
//    layout.addWidget(&lable1);
//    layout.addWidget(&lable2);
//    filewindow.setLayout(&layout);
//    filewindow.show();
    quint32 num;
    quint32 first=1;
    quint64 havesend=0;
    qDebug()<<"start to send";
    //QMessageBox::warning(0,"send","start to send");
    char *text=new char[2048];
    while(num=readfile.read(text,1024)){
        quint32 endnum=8;
        char charstate;
        qDebug()<<"thsi is "<<first<<" size = "<<num;
        do{
            socket->write("send\n");
            socket->write((QString::number(first).toStdString()+'\n').c_str());
            int numberwr=socket->write(QByteArray::fromRawData(text,num));
            //sendsocket.write(text,couts*8+16);
            qDebug()<<"this time"<< first<<" file "<<num<<" has send"<<numberwr;
            socket->waitForBytesWritten();
            if(socket->waitForReadyRead()&&(endnum--)){
                charstate=socket->readAll().toStdString().at(0);
                if(charstate=='1')qDebug()<<QString("fasongshibai\n");
            }else{
                qDebug()<<"lian xu fasong shibai hou\n";
                QMessageBox::warning(0,"SendFile","fail to send;please check the send!");
                readfile.close();
                return false;
            }
        }while(charstate=='1');
        qDebug()<<QString("ok send\n");
        havesend+=num;
        first++;
        //lable2.setText(QString::number(havesend*100/filesize)+"%");
    }
    socket->readAll();
    socket->write("ovo\n");
    socket->waitForBytesWritten();
    if(socket->waitForReadyRead()){
        char charstate=socket->readAll().toStdString().at(0);
        if(charstate!='A'){
            QMessageBox::warning(0,QString("SendFile"),QString("send fail!"));
        }else{
            QMessageBox::warning(0,QString("SendFile"),QString("send success!"));
        }
    }else{
        QMessageBox::warning(0,QString("SendFile"),"fail to send;please check the send!");
        readfile.close();
        delete []text;
        return false;
    }
    delete []text;
    return true;
}

bool FileSR::fileReceive(QTcpSocket *mysocket, quint64 filesize, QString filename, QString username, QString aimusername){
    MySPNPlus myspn;
    myspn.getKey(aimusername,-1);
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
    QFile receivefile(filename);
    if(!receivefile.open(QFile::WriteOnly)){
        QMessageBox::warning(0,"ReceiveFile",QString("fail to create")+filename);
        receivefile.close();
        return false;
    }
    //开始收文件
    quint32 first=1;;
    quint64 havereceive=0;
//    QString strtime;
//    QDateTime time;
//    time = QDateTime::currentDateTime();
//    strtime = time.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<"start to receive fiel ";
    while(mysocket->waitForReadyRead()){
        QString receivetxt=QString::fromStdString(mysocket->readLine().toStdString());
        QString receivefirst=QString::fromStdString(mysocket->readLine().toStdString());receivefirst.chop(1);
        if(receivetxt.compare("send\n")){
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
        havereceive+=blockNum;
        //lable2.setText(QString::number(havereceive*100/receivefilesize)+"%");
        mysocket->readAll();
        mysocket->write("0\n");
        mysocket->waitForBytesWritten();
    }
//    time = QDateTime::currentDateTime();
//    strtime = time.toString("yyyy-MM-dd hh:mm:ss");
//    qDebug()<<"end to receive fiel "<<strtime;
    receivefile.close();
    return true;
}
