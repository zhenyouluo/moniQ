#-------------------------------------------------
#
# Project created by QtCreator 2015-09-26T12:45:28
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT += sql
QT += network
TARGET = moniQ
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tcpscanner.cpp

HEADERS += \
    tcpscanner.h \
    pingerinterface.h
