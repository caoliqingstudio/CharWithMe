#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <string>

enum returnType{FAILCONNECT,SUCCESSCON,FAILSEND,NORETURN,SUCCESS,WRONG_ANSWER};
enum connectType{CONNECT,SENDTEXT,LOGIN,REGISTER,PWRE,ADD,SENDFILE,LISTENING,PWRE_ASK};
enum returnState{TRUE_REQUEST,FALSE_REQUEST};
#endif // STATE_H