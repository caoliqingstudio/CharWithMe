#ifndef CHAT_H
#define CHAT_H

#include <QMainWindow>
#include <QTime>
#include <QFileDialog>
#include "thread.h"
#include "mysocket.h"
#include "state.h"

namespace Ui {
class Chat;
}

class Chat : public QMainWindow
{
    Q_OBJECT

public:
    QString aimuserport;
    QString aimuserip;
    QString *hostip;
    QString *hostport;
    bool statehost;
    QString username;
    QString aimusername;
    MySocket *mysocket;
    QTcpSocket *socket;
    void init();
    explicit Chat(QWidget *parent = 0);
    ~Chat();
    void addInfor(QString name,QString infor,QString time);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Chat *ui;
};

#endif // CHAT_H
