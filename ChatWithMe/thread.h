#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include "state.h"
#include "mysocket.h"

class Thread: public QThread
{
    Q_OBJECT
public:
    Thread(int type);
protected:
    virtual void run();
};

#endif // THREAD_H
