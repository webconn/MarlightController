#-------------------------------------------------
#
# Project created by QtCreator 2014-07-28T15:05:08
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = MarlightController
TEMPLATE = lib

DEFINES += MARLIGHTCONTROLLER_LIBRARY

SOURCES += marlightcontroller.cpp

HEADERS += marlightcontroller.h\
        marlightcontroller_global.h

QMAKE_CXXFLAGS += -std=c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
}
