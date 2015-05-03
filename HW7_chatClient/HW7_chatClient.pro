#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T21:16:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatClient
TEMPLATE = app


SOURCES += main.cpp\
        chatwindow.cpp \
    clientconnectwindow.cpp \
    userlistwindow.cpp

HEADERS  += chatwindow.h \
    clientconnectwindow.h \
    userlistwindow.h

FORMS    += chatwindow.ui \
    clientconnectwindow.ui \
    userlistwindow.ui
