#-------------------------------------------------
#
# Project created by QtCreator 2015-11-29T11:30:44
#
#-------------------------------------------------

QT       -= gui

TARGET = pinglin
TEMPLATE = lib

DEFINES += PINGLIN_LIBRARY

SOURCES += pinglin.cpp

HEADERS += pinglin.h\
        pinglin_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
