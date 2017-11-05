#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <iostream>
#include "server.h"
#include "mysocket.h"
#include "addfriend.h"
#include "chat.h"

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
    QMap<QString,Chat*> chatUserUI;
    QMap<QString,QString> charUserIP;
private slots:
    void on_pushButton_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    AddFriend *AddFriendWindows;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
