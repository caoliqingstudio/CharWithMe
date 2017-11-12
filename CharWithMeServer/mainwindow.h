#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "state.h"
#include "database.h"
#include <QMainWindow>
namespace Ui {
class MainWindow;
}

#include "myserver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    DataBase *db;
    void *myserver;//本来打算用 server
    QString port;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
