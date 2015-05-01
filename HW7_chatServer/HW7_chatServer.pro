#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T20:58:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatServer
TEMPLATE = app


SOURCES += main.cpp\
        serverwindow.cpp \
    SSLServer.cpp \
    serverworker.cpp

HEADERS  += serverwindow.h \
    SSLServer.h \
    serverworker.h

FORMS    += serverwindow.ui
