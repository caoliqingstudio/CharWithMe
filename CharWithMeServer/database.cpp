#include "database.h"

DataBase::DataBase()
{
    db = QSqlDatabase::database("sqlite"); //建立数据库连接
    db.open();
}

DataBase::~DataBase()
{
    db.close();
}

DataBase::DataBase(int start){
    if (QSqlDatabase::contains("sqlite"))
    {
        db = QSqlDatabase::database("sqlite");
        db.open();
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE","sqlite");
        db.setDatabaseName("MyDataBase.db");
        db.setUserName("root");
        db.setPassword("root");
        db.open();
        createTable();
    }
}


//创建数据库表
bool DataBase::createTable()
{
    QSqlQuery query(db);
    bool success=query.exec("create table online(username TEXT,userIP TEXT)");
    success = success&query.exec("create table users(username TEXT,password TEXT,question1 TEXT,"
               "answer1 TEXT,question2 TEXT,answer2 TEXT,question3 TEXT,answer3 TEXT,logo BLOB)");//用户表
    success = success&query.exec("create table offLine(username TEXT,aimusername TEXT,information TEXT,time TEXT,state int)");//离线消息
    success = success&query.exec("create table friends(username TEXT,friendname TEXT,nickname TEXT,state int,time TEXT)");//好友列表

    if(success)
    {
        qDebug() << QObject::tr("数据库表创建成功！\n");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("数据库表创建失败！\n");
        return false;
    }
}



bool DataBase::insertOnline(QString username,QString ip){
    QSqlQuery query(db);
    query.prepare("insert into online values(?, ?)");
    query.bindValue(0, username);
    query.bindValue(1, ip);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("online 插入失败"));
        return false;
    }
    return true;
}

bool DataBase::insertUser(QString username,QString password,QString question1,QString answer1,
                QString question2,QString answer2,QString question3,
                QString answer3,QString logo){
    QSqlQuery query(db);
    query.prepare("insert into users values(?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, username);
    query.bindValue(1, password);
    query.bindValue(2, question1);
    query.bindValue(3, answer1);
    query.bindValue(4, question2);
    query.bindValue(5, answer2);
    query.bindValue(6, question3);
    query.bindValue(7, answer3);
    query.bindValue(8, logo);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("users 插入失败"));
        return false;
    }
    return true;
}

bool DataBase::insertOffLine(QString username,QString aimusername,QString information,
                   QString time,int state){
    QSqlQuery query(db);
    query.prepare("insert into offLine values(?, ?, ?, ?, ?)");
    query.bindValue(0, username);
    query.bindValue(1, aimusername);
    query.bindValue(2, information);
    query.bindValue(3, time);
    query.bindValue(4, state);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("offLine 插入失败"));
        return false;
    }
    return true;
}

bool DataBase::insertFriends(QString username,QString friendname,QString settime,
                   QString nickname,int state){
    QSqlQuery query(db);
    query.prepare("insert into friends values(?, ?, ?, ?, ?)");
    query.bindValue(0, username);
    query.bindValue(1, friendname);
    query.bindValue(2, settime);
    query.bindValue(3, nickname);
    query.bindValue(4, state);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("friends 插入失败"));
        return false;
    }
    return true;
}

bool DataBase::findOnline(QString username,QString *ip){
    QSqlQuery query(db);
    query.prepare("select * from online where username = ?");
    query.bindValue(0,username);
    query.exec();
    QSqlRecord rec = query.record();
    qDebug() << QObject::tr("online 表字段数:" ) << rec.count();
    bool state=false;
    while(query.next())
    {
        state=true;
        *ip=query.value(1).toString();
    }
    return state;
}

int DataBase::onlineNumber(QStringList *users){
    QSqlQuery query(db);
    query.prepare("select * from online");
    query.exec();
    int i=0;
    while(query.next()){
        i++;
        if(users){
            users->append(query.value(0).toString());
        }
    }
    return i;
}

int DataBase::userNumber(QStringList *user){
    QSqlQuery query(db);
    query.exec("select * from users");
    int i=0;
    while(query.next()){
        i++;
        if(user){
            user->append(query.value(0).toString());
        }
    }
    return i;
}

bool DataBase::findUser(QString username,QString *password){
    QSqlQuery query(db);
    if(password){
        query.prepare("select * from users where username = ? and password =?");
        query.bindValue(0,username);
        query.bindValue(1,*password);
    }else{
        query.prepare("select * from users where username = ?");////////////////////////////////////////////
        query.bindValue(0,username);
    }
    query.exec();
    bool returnstate=false;
    if(query.next())
    {
        returnstate=true;
    }
    return returnstate;
}

bool DataBase::findPwReAsk(QString username,QString *question1,QString *question2,QString *question3){
    QSqlQuery query(db);
    query.prepare("select * from users where username = ?");
    query.bindValue(0,username);
    query.exec();
    QSqlRecord rec = query.record();
    qDebug() << QObject::tr("online 表字段数:" ) << rec.count();
    bool returnstate=false;
    if(query.next())
    {
        returnstate=true;
        *question1=query.value(2).toString();
        *question2=query.value(4).toString();
        *question3=query.value(6).toString();
    }
    return returnstate;
}

bool DataBase::findPwRe(QString username,QString password,QString question1,QString answer1,
              QString question2,QString answer2,QString question3,
              QString answer3){
    QSqlQuery query(db);
    query.prepare("update users set password=? where username = ? and answer1 = ? and answer2 = ? and answer3 = ?");
    query.bindValue(0,password);
    query.bindValue(1,username);
    query.bindValue(2,answer1);
    query.bindValue(3,answer2);
    query.bindValue(4,answer3);
    if(query.exec()){

        return true;
    }
    return false;
}

bool DataBase::findFriends(QString username,QStringList *friendnames,QStringList *nickname,
                 QStringList *time,int state){//state 没有使用
    QSqlQuery query(db);
    query.prepare("select * from friends where username = ?");
    query.bindValue(0,username);
    query.exec();
    bool returnstate=false;
    while(query.next()){
        returnstate=true;
        friendnames->append(query.value(1).toString());
        nickname->append(query.value(2).toString());
        time->append(query.value(3).toString());
    }
    return returnstate;
}

bool DataBase::findOffLine(QString aimusername,QStringList *username,QStringList*information,
                 QStringList *time,int state){//state 没有使用啊
    QSqlQuery query(db);
    query.prepare("select * from offLine where aimusername = ?");
    query.bindValue(0,aimusername);
    query.exec();
    bool returnstate=false;
    while(query.next()){
        returnstate=true;
        username->append(query.value(0).toString());
        information->append(query.value(2).toString());
        time->append(query.value(3).toString());
    }
    if(returnstate){
        deleteOffLine(aimusername,1);
    }
    return returnstate;
}


bool DataBase::deleteOnline(QString username,int type){
    QSqlQuery query(db);
    if(type==0){
        query.prepare("delete from online where username = ?");
    }else{
        query.prepare("delete from online where ip = ?");
    }
    query.bindValue(0,username);
    if(query.exec()){
        return true;
    }
    return false;
}

bool DataBase::deleteUser(QString username){
    QSqlQuery query(db);
    query.prepare("delete from users where username = ?");
    query.bindValue(0,username);
    if(query.exec()){
        return true;
    }
    return false;
}

bool DataBase::deleteFriends(QString username,QString friendname,int type){
    QSqlQuery query(db);
    if(type==0){
        query.prepare("delete from friends where username = ? and friendname = ?");
        query.bindValue(0,username);
        query.bindValue(1,friendname);
    }else{
        query.prepare("delete from friends where username = ? or friendname = ?");
        query.bindValue(0,username);
        query.bindValue(1,username);
    }
    if(query.exec()){
        return true;
    }
    return false;
}
bool DataBase::deleteOffLine(QString aimusername,int state,QString username){
    QSqlQuery query(db);
    bool returnstate=false;
    switch (state) {
    case 0:
        query.prepare("delete from offLine where aimusername = ? or username = ?");
        query.bindValue(0,aimusername);
        query.bindValue(1,aimusername);
        returnstate=query.exec();
        break;
    case 1:
        query.prepare("delete from offLine where aimusername = ? ");
        query.bindValue(0,aimusername);
        returnstate=query.exec();
        break;
    case 2:
        query.prepare("delete from offLine where username = ? ");
        query.bindValue(0,aimusername);
        returnstate=query.exec();
        break;
    default:
        query.prepare("delete from offLine where aimusername = ? and username = ?");
        query.bindValue(0,aimusername);
        query.bindValue(1,username);
        returnstate=query.exec();
        break;
    }
    return returnstate;
}

bool DataBase::deleteAll(QString username){
    deleteUser(username);
    deleteOnline(username);
    deleteOffLine(username);
    deleteFriends(username,username,1);
    return true;
}
