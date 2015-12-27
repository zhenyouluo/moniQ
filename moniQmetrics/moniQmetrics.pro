#-------------------------------------------------
#
# Project created by QtCreator 2015-12-27T10:30:53
#
#-------------------------------------------------

QT       += core
QT       += network
QT       += sql
QT       -= gui

TARGET = moniQmetrics
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    networkreader.cpp \
    ../database.cpp \
    objectinstances.cpp

HEADERS += \
    networkreader.h \
    ../database.h \
    objectinstances.h
