#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QApplication a(argc, argv);
    Login login;
    login.show();

    return a.exec();
}
