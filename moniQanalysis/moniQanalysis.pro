#-------------------------------------------------
#
# Project created by QtCreator 2015-11-22T11:25:40
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT       += sql

TARGET = moniQanalysis
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../database.cpp \
    objectinstances2.cpp \
    analyzing.cpp \
    stdinthread.cpp

HEADERS += \
    ../database.h \
    objectinstances2.h \
    analyzing.h \
    stdinthread.h
