#-------------------------------------------------
#
# Project created by QtCreator 2015-09-26T12:45:28
#
#-------------------------------------------------

include(pingwin/pingwin.pri)

QT       += core

QT       -= gui
QT += sql
QT += network
TARGET = moniQ
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    networkdiscoverer.cpp \
    pinger.cpp \
    ipv4_address.cpp \
    networkreader.cpp \
    objectinstances.cpp

HEADERS += \
    pingerinterface.h \
    networkdiscoverer.h \
    pinger.h \
    ipv4_address.h \
    networkreader.h \
    objectinstances.h


