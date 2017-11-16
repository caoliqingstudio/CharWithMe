#include "filesr.h"
#define FAST_NUM 5
#define A_BLOCK_SIZE 1024*1024*100

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
        //QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
        readfile.close();
        return false;
    }
    if(result.at(2)!=('0'+TRUE_REQUEST)){
        //QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
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
    //quint64 havesend=0;
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
            //socket->write((QString::number(first).toStdString()+'\n').c_str());
            int numberwr=socket->write(QByteArray::fromRawData(text,num));
            //sendsocket.write(text,couts*8+16);
            //qDebug()<<"this time"<< first<<" file "<<num<<" has send"<<numberwr;
            socket->waitForBytesWritten();
            if(socket->waitForReadyRead()&&(endnum--)){
                charstate=socket->readAll().toStdString().at(0);
                if(charstate=='1') qDebug()<<QString("fasongshibai\n");
            }else{
                qDebug()<<"lian xu fasong shibai hou\n";
                //QMessageBox::warning(0,"SendFile","fail to send;please check the send!");//
                readfile.close();
                return false;
            }
        }while(charstate=='1');
        //qDebug()<<QString("ok send\n");
        //havesend+=num;
        //first++;
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
    QString strtime;
    QDateTime time;
    time = QDateTime::currentDateTime();
    strtime = time.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<"start to receive fiel "<<strtime;
    while(mysocket->waitForReadyRead()){
        QString receivetxt=QString::fromStdString(mysocket->readLine().toStdString());
        QByteArray bytearry= mysocket->readAll();
        //QString receivefirst=QString::fromStdString(mysocket->readLine().toStdString());receivefirst.chop(1);
        if(receivetxt.compare("send\n")){
            if(receivetxt.compare("ovo\n")){
                return false;
            }
            mysocket->write("A\n");
            mysocket->waitForBytesWritten();
            break;
        }/*else if(receivefirst.toInt()!=first){
            mysocket->readAll();
            //qDebug()<<"fail to receive the block = "<<receivefirst;
            mysocket->write("1\n");
            mysocket->waitForBytesWritten();
            continue;
        }*/
        int blocknumR=bytearry.size();
        char *receive=bytearry.data();
        int couts=blocknumR/8;
        for(int i=0;i<couts;i++){
            *(((quint64*)receive)+i)=myspn.decrypt(*(((quint64*)receive)+i));
        }
        quint64 blockNum=receivefile.write(bytearry);
        //qDebug()<<"have receive "<<first<<" receice "<<blocknumR<<" file "<<blockNum;
        havereceive+=blockNum;
        //lable2.setText(QString::number(havereceive*100/receivefilesize)+"%");
        mysocket->readAll();
        mysocket->write("0\n");
        mysocket->waitForBytesWritten();
        first++;
    }
    time = QDateTime::currentDateTime();
    strtime = time.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<"end to receive fiel "<<strtime;
    receivefile.close();
    return true;
}

bool FileSR::fileSendFast(QTcpSocket *socket, QString filename, QString username, QString aimusername){
    socket->readAll();
    qDebug()<<"send wenjian yubei"<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
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
        //QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
        readfile.close();
        return false;
    }
    if(result.at(2)!=('0'+TRUE_REQUEST)){
        //QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
        readfile.close();
        return false;
    }
    QTcpSocket *socketArry[FAST_NUM];
    for(int i=0;i<FAST_NUM;++i){
        socketArry[i]=new QTcpSocket;
        socketArry[i]->connectToHost(socket->peerAddress(),FILE_PORT);
        if(!socketArry[i]->waitForConnected()){
            qDebug()<<"fail to connect this is="<<i;
            return false;
        }
    }
    bool stillFile=true;
    char *text=new char[BLOCK_SIZE+1024];
    int end=FAST_NUM;
    int first=1;
    qDebug()<<"fasong wenjian kaishi"<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    while(stillFile){
        first++;
        for(int i=0;i<FAST_NUM;i++){
            int num;
            if(num=readfile.read(text,BLOCK_SIZE)){
                socketArry[i]->write("send\n");
                socketArry[i]->write(QByteArray::fromRawData(text,num));
                if(!socketArry[i]->waitForBytesWritten()){
                    qDebug()<<"fail to send = "<<i<<num;
                    return false;
                }
            }else{
                socketArry[i]->write("ovo\n");
                socketArry[i]->write(QByteArray::fromRawData(text,num));
                if(!socketArry[i]->waitForBytesWritten()){
                    qDebug()<<"fail to send end "<<i<<num;
                    return false;
                }
                end=i+1;
                stillFile=false;
                break;
            }
        }
        for(int i=0;i<end;i++){
            if(socketArry[i]->waitForReadyRead()){
                std::string reString=socketArry[i]->readAll().toStdString();
                char c=reString.at(0);
                if(c!='0'){
                    qDebug()<<"wrong to  send return = "<<i;
                    return false;
                }
                //qDebug()<<"send sucess "<<first++<<i<<QString::fromStdString(reString);
            }else{
                qDebug()<<"fail to  send return = "<<i;
                return false;
            }
        }
    }
    qDebug()<<"fasong wenjian jieshu"<<first<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    readfile.close();
    delete []text;
    return true;
}

bool FileSR::fileReceiveFast(QTcpSocket *mysocket, quint64 filesize, QString filename, QString username, QString aimusername){
    MySPNPlus myspn;
    qDebug()<<"receive wenjian yubei"<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    myspn.getKey(aimusername,-1);
    char result[]="0\n0\n";
    result[0]+=SENDFILE;
    result[2]+=TRUE_REQUEST;
    mysocket->readAll();//清空缓存
    QTcpSocket * socketArry[FAST_NUM];
    QTcpServer server;
    server.listen(QHostAddress::Any,FILE_PORT);
    mysocket->write(result);
    mysocket->waitForBytesWritten();
    for(int i=0;i<FAST_NUM;++i){
        if(server.waitForNewConnection(30000)){
            socketArry[i]=server.nextPendingConnection();
            socketArry[i]->readAll();
            qDebug()<<"listen ok";
        }else{
            qDebug()<<"cant listen the file "<<i;
            return false;
        }
    }
    quint32 first=1;
    QString strtime;
    QDateTime time;
    QFile receivefile(filename);
    if(!receivefile.open(QFile::WriteOnly)){
        QMessageBox::warning(0,"ReceiveFile",QString("fail to create")+filename);
        receivefile.close();
        return false;
    }
    qDebug()<<"jie shou wenjian kaishi"<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    bool stillFile=true;
    int end=FAST_NUM;
    while(stillFile){
        QByteArray text[FAST_NUM];
        for(int i=0;i<FAST_NUM;i++){
            if(socketArry[i]->waitForReadyRead()){
                std::string textString=socketArry[i]->readLine().toStdString();
                text[i]=socketArry[i]->readAll();
                if(textString.compare("send\n")){
                    if(!textString.compare("ovo\n")){
                        stillFile=false;
                        end=i+1;
                        break;
                    }
                    qDebug()<<"fail to receive = "<<i<<QString::fromStdString(textString);
                    return false;
                }
                //qDebug()<<"success receive "<<first++<<" this i ="<<i<<"block size="<<text[i].size();
            }else{
                    qDebug()<<"fail to receive "<<i;
                    return false;
            }
        }
        for(int i=0;i<end;i++){
            int num=receivefile.write(text[i]);
            socketArry[i]->readAll();
            socketArry[i]->write("0\n");
            if(!socketArry[i]->waitForBytesWritten()){
                qDebug()<<"wrong to  receive return = "<<i;
                return false;
            }
        }
        first++;
    }
    qDebug()<<"jie shou wenjian jieshu"<<first<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    receivefile.close();
    return true;
}


bool FileSR::fileSendFast_A(QTcpSocket *socket, QString filename, QString username, QString aimusername){
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
    int tempfile=filesize%8;
    if(tempfile){
        filesize+=(8-tempfile);
    }
    char *text=new char[filesize+1024];
    memset(text,0,filesize);
    quint32 num=readfile.read(text,filesize);
//    if(num!=filesize){
//        qDebug()<<"failt "<<num<<filesize;
//        return false;
//    }
    int couts=num/8+1;
    for(int i=0;i<couts;i++){
        *(((unsigned long long*)text)+i)=myspn.encrypt(*(((unsigned long long*)text)+i));
    }
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
        //QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
        readfile.close();
        return false;
    }
    if(result.at(2)!=('0'+TRUE_REQUEST)){
        //QMessageBox::warning(0,"SendFile","fail to send;please check the user!");
        readfile.close();
        return false;
    }
    QTcpSocket thisSocket;
    QHostAddress host(socket->peerAddress().toString());
    socket=&thisSocket;
    thisSocket.connectToHost(host,FILE_PORT);
    if(!thisSocket.waitForConnected()){
        qDebug()<<"fail to connect this is=";
        //return false;
    }
    if(!thisSocket.waitForReadyRead()){
        qDebug()<<"no messgage";
    }
    qDebug()<<"return = "<<QString(thisSocket.readAll());
    //quint64 havesend=0;
    qDebug()<<"start to send";
    //quint32 blockSize= A_BLOCK_SIZE;
//    while(num=readfile.read(text,filesize)){
//        char *numchar=(char *)&num;
//        qDebug()<<num<<filesize;
//        socket->write("send\n");
//        socket->write(QByteArray::fromRawData(numchar,4));
//        socket->write(QByteArray::fromRawData(text,num));
//        if(!socket->waitForBytesWritten()){
//            qDebug()<<"fail send";
//            return false;
//        }
//    }

    qDebug()<<"send "<<num<<filesize;
    //socket->readAll();
    //socket->write("send\n");
    //socket->write(QByteArray::fromRawData(numchar,4));
    QString timestart=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    socket->write(QByteArray::fromRawData(text,filesize));
    if(!socket->waitForBytesWritten(300000)){
        qDebug()<<"fail send";
        return false;
    }
    QString timeend=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QMessageBox::warning(0,"fileSend","start ="+timestart+"\nend = "+timeend);
//    socket->write("ovo\n");
//    if(!socket->waitForBytesWritten()){
//        qDebug()<<"fail send end";
//        return false;
//    }
    delete []text;
    return true;
}

bool FileSR::fileReceiveFast_A(QTcpSocket *mysocket, quint64 filesize, QString filename, QString username, QString aimusername){
    MySPNPlus myspn;
    myspn.getKey(aimusername,-1);
    char result[]="0\n0\n";
    result[0]+=SENDFILE;
    result[2]+=TRUE_REQUEST;
    mysocket->readAll();//清空缓存
    QTcpServer server;
    server.listen(QHostAddress::Any,FILE_PORT);
    mysocket->write(result);
    mysocket->waitForBytesWritten();
    QTcpSocket *newSocket;
    if(server.waitForNewConnection(30000)){
        qDebug()<<"success connect";
        newSocket=server.nextPendingConnection();
    }else{
        qDebug()<<"connect fail";
        return false;
    }
    QFile receivefile(filename+"_temp");
    if(!receivefile.open(QFile::WriteOnly)){
        QMessageBox::warning(0,"ReceiveFile",QString("fail to create")+filename);
        receivefile.close();
        return false;
    }
    newSocket->readAll();
    newSocket->write("1\n");
    newSocket->waitForBytesWritten();
    //开始收文件
    QString strtime1,strtime2;
    QDateTime time;
    time = QDateTime::currentDateTime();
    strtime1 = time.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<"start to receive fiel "<<strtime1<<filesize;
    while(filesize>0){
        if(!newSocket->waitForReadyRead()){
            qDebug()<<"fail to receive ";
            return false;
        }
        QByteArray text=newSocket->read(filesize);
        filesize=filesize-text.count();
        receivefile.write(text);
        //qDebug()<<filesize<<text.count()<<text.size();
    }
//    while(filesize>0){
//        mysocket->waitForReadyRead(3000);
//        std::string restr=mysocket->readLine().toStdString();
//        qDebug()<<QString::fromStdString(restr);
//        if(restr.compare("send\n")){
//            qDebug()<<"receive = "<<QString::fromStdString(restr);
//            if(!restr.compare("ovo\n")){
//                break;
//            }
//            qDebug()<<"receive fail";
//            return false;
//        }
//        quint32 blocksize=*(quint32*)((mysocket->read(4)).data());
//        filesize-=blocksize;
//        qDebug()<<"blocksize="<<blocksize;
//        while(blocksize){
//            QByteArray txt=mysocket->read(blocksize);
//            blocksize-=txt.size();
//            receivefile.write(txt);
//        }
//    }
    time = QDateTime::currentDateTime();
    strtime2 = time.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<"end to receive fiel "<<strtime2;
    QMessageBox::warning(0,"fileRecive","start ="+strtime1+"\nend = "+strtime2);
    receivefile.close();
    QFile tempfile(filename+"_temp");
    if(!tempfile.open(QFile::ReadOnly)){
        QMessageBox::warning(0,"ReceiveFile",QString("fail to read")+filename);
        tempfile.close();
        return false;
    }
    QFile endfile(filename);
    if(!endfile.open(QFile::WriteOnly)){
        QMessageBox::warning(0,"ReceiveFile",QString("fail to create")+filename);
        endfile.close();
        return false;
    }
    char endText[16];
    while(tempfile.read(endText,8)){
        *(unsigned long long *)endText=myspn.decrypt(*(unsigned long long *)endText);
        endfile.write(endText,8);
    }
    endfile.close();
    tempfile.remove();
    tempfile.close();
    return true;
}
