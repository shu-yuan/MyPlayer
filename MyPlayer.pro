#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T20:39:54
#
#-------------------------------------------------

QT       += core gui
QT += phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mywidget.cpp \
    myplaylist.cpp

HEADERS  += mywidget.h \
    myplaylist.h

RESOURCES += \
    pics.qrc
