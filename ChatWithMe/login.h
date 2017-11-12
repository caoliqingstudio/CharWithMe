#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "register.h"
#include "pwretrieval.h"
#include "mainwindow.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    QString *port;
    QString *mainUserName;
    QString *ip;
    explicit Login(QWidget *parent = 0);
    ~Login();
    void init();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_findpwdButton_clicked();

    void on_pushButton_4_clicked();

private:
    PwRetrieval *pwRetrievalWindows;
    Register *registerWindows;
    Ui::Login *ui;
};

#endif // LOGIN_H
