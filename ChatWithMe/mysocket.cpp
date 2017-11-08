#include "mysocket.h"

MySocket::MySocket(QString ip,QString port)
{
    QHostAddress address(ip);
    myconnect.connectToHost(address,port.toUInt());
    if(!myconnect.waitForConnected()){
        if(!myconnect.waitForConnected()){
            QMessageBox::warning(0,tr("ByCLQ"),tr("Fail to connect to Host!"));
            connectState =FAILCONNECT;
        }else{
            connectState =SUCCESSCON;
        }
    }else{
        connectState =SUCCESSCON;
    }
}

int MySocket::sendText(int type, QString string,QString username,QString aimusername){
    if(connectState==FAILCONNECT){
        return FAILCONNECT;
    }
    QString typeString=QString::number(type);
    QString strtime;
    QDateTime time;

    time = QDateTime::currentDateTime();

    strtime = time.toString("yyyy-MM-dd hh:mm:ss");
    string=typeString+"\n"+username+"\n"+aimusername+"\n"+strtime+"\n"+string;
    myconnect.write(string.toStdString().c_str());
    if(!myconnect.waitForBytesWritten()){
        std::cout<<"nothing \n"<<std::endl;
        return FAILSEND;
    }/*
    QByteArray infor=myconnect.readAll();
    std::cout<<"youshenme  ne \n"<<infor.toStdString()<<std::endl;
    QString result=QString::fromStdString(infor.toStdString());*/
    return SUCCESS;
}


int MySocket::sendRegister(QString username,QString password,QString quest1,QString answ1,
              QString quest2,QString answ2,QString quest3,QString answ3){
    int type=REGISTER;
    QString string;
    QString typeString=QString::number(type);
    QByteArray byte_array=password.toStdString().c_str();
    QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
    QString md5 = hash_byte_array.toHex();
//    for(int i=0;i<md5.length();i++){
//        if((md5.toStdString().at(i))=='\n'||(md5.toStdString().at(i))=='\0'){
//            md5[i]='*';
//        }
//    }
    string=typeString+"\n"+username+"\n"+md5+"\n"+quest1+"\n"+answ1+"\n"+quest2+"\n"+answ2+"\n"+quest3+"\n"+answ3+"\n";
    myconnect.write(string.toStdString().c_str());
    if(!myconnect.waitForBytesWritten()){
        std::cout<<"nothing \n"<<std::endl;
        return FAILSEND;
    }
    if(!myconnect.waitForReadyRead()){
        std::cout<<"nothing \n"<<std::endl;
        return NORETURN;
    }
    std::string result=myconnect.readAll().toStdString();
    std::cout<<"result = "<<result<<std::endl;
    if(result.at(0)!=('0'+REGISTER)){
        qDebug()<<"wrong answer";
        return WRONG_ANSWER;
    }
    if(result.at(2)!=('0'+TRUE_REQUEST)){
        qDebug()<<"wrong answer reginster";
        return WRONG_ANSWER;
    }
    return SUCCESS;
}

int MySocket::sendLogin(QString username,QString password){
    int type=LOGIN;
    QString string;
    QString typeString=QString::number(type);
    QByteArray byte_array=password.toStdString().c_str();
    QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
    QString md5 = hash_byte_array.toHex();
    string=typeString+"\n"+username+"\n"+md5+"\n";
    myconnect.write(string.toStdString().c_str());
    if(!myconnect.waitForBytesWritten()){
        std::cout<<"nothing \n"<<std::endl;
        return FAILSEND;
    }
    if(!myconnect.waitForReadyRead()){
        std::cout<<"nothing \n"<<std::endl;
        return NORETURN;
    }
    std::string result=myconnect.readAll().toStdString();
    if(result.at(0)!=('0'+LOGIN)){
        qDebug()<<"wrong answer";
        return WRONG_ANSWER;
    }
    if(result.at(2)!=('0'+TRUE_REQUEST)){
        qDebug()<<"wrong answer reginster";
        return WRONG_ANSWER;
    }
    return SUCCESS;
}


int MySocket::sendPwRetrieval(QString username,QString newpassword,QString quest1,QString answ1,
              QString quest2,QString answ2,QString quest3,QString answ3){
    int type=PWRE;
    QString string;
    QString typeString=QString::number(type);
    QByteArray byte_array=newpassword.toStdString().c_str();
    QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
    QString md5 = hash_byte_array.toHex();
    string=typeString+"\n"+username+"\n"+md5+"\n"+quest1+"\n"+answ1+"\n"+quest2+"\n"+answ2+"\n"+quest3+"\n"+answ3+"\n";
    myconnect.write(string.toStdString().c_str());
    if(!myconnect.waitForBytesWritten()){
        std::cout<<"nothing \n"<<std::endl;
        return FAILSEND;
    }
    if(!myconnect.waitForReadyRead()){
        std::cout<<"nothing \n"<<std::endl;
        return NORETURN;
    }
    std::string result=myconnect.readAll().toStdString();
    if(result.at(0)!=('0'+PWRE)){
        qDebug()<<"wrong answer";
        return WRONG_ANSWER;
    }
    if(result.at(2)!=('0'+TRUE_REQUEST)){
        qDebug()<<"wrong answer reginster";
        return WRONG_ANSWER;
    }
    return SUCCESS;
}

int MySocket::sendPwRetrievalAsk(QString username,QStringList *questions){
    int type=PWRE_ASK;
    QString string;
    QString typeString=QString::number(type);
    string=typeString+"\n"+username+"\n";
    myconnect.write(string.toStdString().c_str());
    if(!myconnect.waitForBytesWritten()){
        std::cout<<"nothing \n"<<std::endl;
        return FAILSEND;
    }
    if(!myconnect.waitForReadyRead()){
        std::cout<<"nothing \n"<<std::endl;
        return NORETURN;
    }
    std::string result=myconnect.readLine().toStdString();
    if(result.at(0)!=('0'+PWRE_ASK)){
        qDebug()<<"wrong answer";
        return WRONG_ANSWER;
    }
    result=myconnect.readLine().toStdString();
    if(result.at(0)!=('0'+TRUE_REQUEST)){
        qDebug()<<"wrong answer pwretrievalask";
        return WRONG_ANSWER;
    }
    questions->clear();
    QString question;
    question=QString::fromStdString(myconnect.readLine().toStdString());question.chop(1);
    questions->append(question);
    question=QString::fromStdString(myconnect.readLine().toStdString());question.chop(1);
    questions->append(question);
    question=QString::fromStdString(myconnect.readLine().toStdString());question.chop(1);
    questions->append(question);
    return SUCCESS;
}

//检测tcp连接的
int MySocket::checkAndConnect(){
    return SUCCESS;
}

MySocket::~MySocket(){
    myconnect.disconnect();
    //myconnect.waitForDisconnected();
}

