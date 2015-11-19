#-------------------------------------------------
#
# Project created by QtCreator 2015-11-15T12:52:10
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT       += sql

TARGET = moniQscheduling
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    stdinthread.cpp \
    scheduling.cpp \
    objectinstances2.cpp \
    ../database.cpp


HEADERS += \
    stdinthread.h \
    scheduling.h \
    objectinstances2.h \
    ../database.h

