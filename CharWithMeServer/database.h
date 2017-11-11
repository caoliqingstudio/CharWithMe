#ifndef DATABASE_H
#define DATABASE_H

#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>

class DataBase
{
public:
    DataBase();
    DataBase(int start);//程序运行时候使用
    ~DataBase();
    bool insertOnline(QString username,QString ip);       //插入在线用户
    bool insertUser(QString username,QString password,QString question1,QString answer1,
                    QString question2,QString answer2,QString question3,
                    QString answer3,QString logo="LOGO");   //插入新用户
    bool insertOffLine(QString username,QString aimusername,QString information,
                       QString time,int state=0);//插入离线消息
    bool insertFriends(QString username,QString friendname,QString settime,
                       QString nickname="李华",int state =0);//添加好友

    bool findOnline(QString username,QString *ip);//在线用户ip
    bool findUser(QString username,QString *password=NULL);//查询用户 或者登陆
    bool findPwReAsk(QString username,QString *question1,QString *question2,QString *question3);
    bool findPwRe(QString username,QString password,QString question1,QString answer1,
                  QString question2,QString answer2,QString question3,
                  QString answer3);//插入用户
    bool findFriends(QString username,QStringList *friendnames,QStringList *nickname,
                     QStringList *time,int state=0);//查找好友
    bool findOffLine(QString aimusername,QStringList *username,QStringList*information,
                     QStringList *time,int state=0);//离线消息
    int onlineNumber(QStringList *user=NULL);
    int userNumber(QStringList *user=NULL);
    //没有写 更新消息的地方
    bool deleteOnline(QString username,int type=0);//type=0 是username 否则是 ip
    bool deleteUser(QString username);
    bool deleteFriends(QString username,QString friendname,int type=0);
    bool deleteOffLine(QString aimusername,int state=0,QString username="username");//state = 0 删除 aim 和 user
                                                                                //1 aim 2 user  其他 user&aim
    bool deleteAll(QString username);
private:
    bool createTable();       //创建数据库表
    QSqlDatabase db;
};

#endif // DATABASE_H

