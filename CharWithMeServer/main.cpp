#include "mainwindow.h"
#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
