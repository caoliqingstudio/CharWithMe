#ifndef CHAT_H
#define CHAT_H

#include <QMainWindow>
#include "mysocket.h"
#include "state.h"

namespace Ui {
class Chat;
}

class Chat : public QMainWindow
{
    Q_OBJECT

public:
    QString *port;
    QString *ip;
    explicit Chat(QWidget *parent = 0);
    ~Chat();

private:
    Ui::Chat *ui;
};

#endif // CHAT_H
