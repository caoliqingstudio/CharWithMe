#include "thread.h"

Thread::Thread(int type)
{
    this->type=type;
}

void Thread::run(){
    switch(type){
    case CONNECT:
    case SENDTEXT:
    case LOGIN:
    case REGISTER:
    case PWRE:
    case ADD:
    case SENDFILE:
    case SERVER:
    default:
        break;
    }
}
