#include "thread.h"

Thread::Thread(int type)
{
    switch(type){
    case CONNECT:
    case SENDTEXT:
    case LOGIN:
    case REGISTER:
    case PWRE:
    case ADD:
    case SENDFILE:
    default:
        break;
    }
}

void Thread::run(){

}
