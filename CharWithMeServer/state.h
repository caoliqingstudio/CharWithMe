#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <string>

enum returnType{FAILCONNECT,SUCCESSCON,FAILSEND,NORETURN,SUCCESS,WRONG_ANSWER};
enum connectType{CONNECT,SENDTEXT,LOGIN,REGISTER,PWRE,ADD,SENDFILE,LISTENING,PWRE_ASK,SERVER};
//CONNECT, ：
//SENDTEXT,：发送文本信息
//LOGIN,   ：登陆
//REGISTER,：注册
//PWRE,    ：找回密码
//ADD,     ：添加好友
//SENDFILE,：发送文件
//LISTENING：查询好友ip
//PWRE_ASK,：找回密码请求问题
//SERVER   :服务器信号
enum returnState{TRUE_REQUEST,FALSE_REQUEST};
#endif // STATE_H
