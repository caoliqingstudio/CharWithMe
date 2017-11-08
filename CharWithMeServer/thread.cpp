#include "thread.h"

Thread::Thread()
{
}

void Thread::run(){    
    if(type==0){
        connect(myconnect,SIGNAL(readyRead()),((Server *)myserver),SLOT(readMessage()));
        connect(myconnect,SIGNAL(disconnected()),this,SLOT(quit()));
        connect(myconnect,SIGNAL(disconnected()),myconnect,SLOT(deleteLater()));
        connect(myconnect,SIGNAL(disconnected()),this,SLOT(deleteLater()));
    }
    exec();
}
