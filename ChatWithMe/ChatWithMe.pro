#-------------------------------------------------
#
# Project created by QtCreator 2017-11-02T08:42:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatWithMe
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    server.cpp \
    login.cpp \
    register.cpp \
    pwretrieval.cpp \
    addfriend.cpp \
    mysocket.cpp \
    thread.cpp \
    myspnplus.cpp \
    filesr.cpp \
    mainwindow.cpp \
    chat.cpp

HEADERS  += \
    server.h \
    login.h \
    register.h \
    pwretrieval.h \
    addfriend.h \
    mysocket.h \
    state.h \
    thread.h \
    myspnplus.h \
    filesr.h \
    mainwindow.h \
    chat.h

FORMS    += mainwindow.ui \
    login.ui \
    register.ui \
    pwretrieval.ui \
    addfriend.ui \
    chat.ui

RESOURCES += \
    picture.qrc
