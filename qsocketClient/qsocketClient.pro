#-------------------------------------------------
#
# Project created by QtCreator 2014-08-06T17:04:08
#
#-------------------------------------------------

QT       += network core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qsocketClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myscrollarea.cpp

HEADERS  += mainwindow.h \
    myscrollarea.h

FORMS    += mainwindow.ui
