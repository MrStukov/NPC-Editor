#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T15:48:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11
TARGET = NPCFileEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    npcfileeditorholder.cpp

HEADERS  += mainwindow.h \
    npcfileeditorholder.h

FORMS    += mainwindow.ui
