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
    QString *ip;
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_findpwdButton_clicked();

    void on_pushButton_4_clicked();

private:
    void init();
    PwRetrieval *pwRetrievalWindows;
    Register *registerWindows;
    Ui::Login *ui;
};

#endif // LOGIN_H
