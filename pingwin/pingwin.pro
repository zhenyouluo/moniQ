#-------------------------------------------------
#
# Project created by QtCreator 2015-10-01T19:27:28
#
#-------------------------------------------------

QT       -= gui

TARGET = pingwin
TEMPLATE = lib

DEFINES += PINGWIN_LIBRARY

SOURCES += pingwin.cpp

HEADERS += pingwin.h \
           pingwin_global.h
LIBS +=  -lws2_32 \
         -liphlpapi
