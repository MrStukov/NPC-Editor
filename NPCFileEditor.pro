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
    Libraries/Tinyxml2/tinyxml2.cpp \
    dialogeditor.cpp \
    npceditorholder.cpp \
    conditionwidget.cpp

HEADERS  += mainwindow.h \
    Libraries/Tinyxml2/tinyxml2.h \
    dialogeditor.h \
    npceditorholder.h \
    conditionwidget.h

FORMS    += mainwindow.ui \
    dialogeditor.ui \
    conditionwidget.ui
