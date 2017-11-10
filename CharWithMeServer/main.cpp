#include "mainwindow.h"
#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QString::fromLocal8Bit ("中文");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
