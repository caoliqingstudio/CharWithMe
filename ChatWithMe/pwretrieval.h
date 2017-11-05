#ifndef PWRETRIEVAL_H
#define PWRETRIEVAL_H

#include <QMainWindow>
#include "state.h"
#include "mysocket.h"

namespace Ui {
class PwRetrieval;
}

class PwRetrieval : public QMainWindow
{
    Q_OBJECT

public:
    QString *port;
    QString *ip;
    explicit PwRetrieval(QWidget *parent = 0);
    ~PwRetrieval();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_5_editingFinished();

private:
    void init();
    Ui::PwRetrieval *ui;
};

#endif // PWRETRIEVAL_H
