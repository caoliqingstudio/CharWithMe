#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>
#include <QMessageBox>
#include "state.h"
#include "mysocket.h"

namespace Ui {
class Register;
}

class Register : public QMainWindow
{
    Q_OBJECT

public:
    QString *port;
    QString *ip;
    explicit Register(QWidget *parent = 0);
    ~Register();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_editingFinished();

private:
    void *login;
    void init();
    Ui::Register *ui;
};

#endif // REGISTER_H
