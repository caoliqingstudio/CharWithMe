#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QListWidgetItem>
#include <iostream>
#include "server.h"
#include "mysocket.h"
#include "addfriend.h"
#include "filesr.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString *port;
    QString *ip;
    QString username;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
    void byAddChat(QString fromname,QString infor,QString time,QTcpSocket*socket=NULL);
    void byAddChat(const QString fromname, QTcpSocket *socket=NULL);
    void addFriendList(QString name);
    QMap<QString,void*> chatUserUI;
    QMap<QString,QString> charUserIP;
private slots:
    void on_pushButton_clicked();

    void addItemFriend();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void deleteChat();

private:
    void *server;
    AddFriend *AddFriendWindows;
    MySocket *mysocket;
    QStringList friendList,nicknameList,timeFriendList;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
