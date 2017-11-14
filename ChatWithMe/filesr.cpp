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
    //qDebug()<<"will send file = "<<filename;
    filesize=readfile.size();
    QString sendtext=QString::number(SENDFILE)+"\n"+username+"\n"
          +aimusername+"\n"+filename+"\n"+QString::number(filesize)+"\n"+strtime+"\n";
    //qDebug()<<"\nfirst send to aim= "<<sendtext<<"\n\n";
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
    //qDebug()<<"start to send";
    //QMessageBox::warning(0,"send","start to send");
    char *text=new char[2048];
    while(num=readfile.read(text,1024)){
        quint32 endnum=8;
        char charstate;
        int couts=num/8;
        if(num%8){
            text[num]=0;text[num+1]=0;text[num+2]=0;text[num+3]=0;
            text[num+4]=0;text[num+5]=0;text[num+6]=0;
            num++;
        }
        for(int i=0;i<couts;i++){
            *(((quint64*)text)+i)=myspn.encrypt(*(((quint64*)text)+i));
        }
        //qDebug()<<"thsi is "<<first<<" size = "<<num;
        do{
            socket->write("send\n");
            socket->write((QString::number(first).toStdString()+'\n').c_str());
            int numberwr=socket->write(QByteArray::fromRawData(text,couts*8));
            //sendsocket.write(text,couts*8+16);
            //qDebug()<<"this time"<< first<<" file "<<num<<" has send"<<numberwr;
            socket->waitForBytesWritten();
            if(socket->waitForReadyRead()&&(endnum--)){
                charstate=socket->readAll().toStdString().at(0);
                //if(charstate=='1')//qDebug()<<QString("fasongshibai\n");
            }else{
                //qDebug()<<"lian xu fasong shibai hou\n";
                //QMessageBox::warning(0,"SendFile","fail to send;please check the send!");
                readfile.close();
                return false;
            }
        }while(charstate=='1');
        //qDebug()<<QString("ok send\n");
        havesend+=num;
        first++;
        //lable2.setText(QString::number(havesend*100/filesize)+"%");
    }
    socket->readAll();
    socket->write("ovo\n");
    socket->waitForBytesWritten();
    if(socket->waitForReadyRead()){
        char charstate=socket->readAll().toStdString().at(0);
        readfile.close();
        if(charstate!='A'){
            //QMessageBox::warning(0,QString("SendFile"),QString("send fail!"));
            return false;
        }else{
            //QMessageBox::warning(0,QString("SendFile"),QString("send success!"));
        }
    }else{
        //QMessageBox::warning(0,QString("SendFile"),"fail to send;please check the send!");
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
    //qDebug()<<"start to receive fiel ";
    while(mysocket->waitForReadyRead()){
        QString receivetxt=QString::fromStdString(mysocket->readLine().toStdString());
        QString receivefirst=QString::fromStdString(mysocket->readLine().toStdString());receivefirst.chop(1);
        if(receivetxt.compare("send\n")){
            mysocket->write("A\n");
            mysocket->waitForBytesWritten();
            break;
        }else if(receivefirst.toInt()!=first){
            mysocket->readAll();
            //qDebug()<<"fail to receive the block = "<<receivefirst;
            mysocket->write("1\n");
            mysocket->waitForBytesWritten();
        }
        QByteArray bytearry= mysocket->readAll();
        int blocknumR=bytearry.size();
        char *receive=bytearry.data();
        int couts=blocknumR/8;
        for(int i=0;i<couts;i++){
            *(((quint64*)receive)+i)=myspn.decrypt(*(((quint64*)receive)+i));
        }
        quint64 blockNum=receivefile.write(bytearry);
        //qDebug()<<"have receive "<<first++<<" receive"<<receivefirst<<" receice "<<blocknumR<<" file "<<blockNum<<" blocksize "<<strlen(receive);
        havereceive+=blockNum;
        //lable2.setText(QString::number(havereceive*100/receivefilesize)+"%");
        mysocket->readAll();
        mysocket->write("0\n");
        mysocket->waitForBytesWritten();
    }
//    time = QDateTime::currentDateTime();
//    strtime = time.toString("yyyy-MM-dd hh:mm:ss");
//    //qDebug()<<"end to receive fiel "<<strtime;
    receivefile.close();
    return true;
}
