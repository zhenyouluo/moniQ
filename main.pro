#-------------------------------------------------
#
# Project created by QtCreator 2015-09-26T12:45:28
#
#-------------------------------------------------

QT       += core
QT       += websockets

QT       -= gui
QT += sql
QT += network
TARGET = moniQ
CONFIG   += console
CONFIG   += thread
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    networkdiscoverer.cpp \
    ipv4_address.cpp \
    objectinstances.cpp \
    commandserver.cpp \
    database.cpp \
    processcontroller.cpp \
    resolver.cpp

HEADERS += \
    networkdiscoverer.h \
    ipv4_address.h \
    objectinstances.h \
    commandserver.h \
    database.h \
    processcontroller.h \
    resolver.h

DISTFILES +=



