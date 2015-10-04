#-------------------------------------------------
#
# Project created by QtCreator 2015-10-01T19:27:28
#
#-------------------------------------------------

QT       -= gui

TARGET = pingwin
TEMPLATE = lib

DEFINES += PINGWIN_LIBRARY

SOURCES += pingwin/pingwin.cpp

HEADERS += pingwin/pingwin.h\
        pingwin/pingwin_global.h
LIBS +=  -lws2_32

