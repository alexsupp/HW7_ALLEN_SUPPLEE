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
    SSLServer.cpp

HEADERS  += serverwindow.h \
    SSLServer.h

FORMS    += serverwindow.ui

OTHER_FILES += \
    cacert.pem \
    privkey.pem
