#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T16:51:26
#
#-------------------------------------------------

QT       += core gui

TARGET = RiverModelGUI
TEMPLATE = app

CONFIG += static

win32:QMAKE_LFLAGS_RELEASE += -static -static-libgcc -static-libstdc++

SOURCES += main.cpp \
    model/rivermodel.cpp \
    model/status.cpp \
    model/globals.cpp \
    model/cleanup.cpp \
    model/color.cpp \
    model/dump.cpp \
    model/go.cpp \
    model/patch.cpp \
    model/pred.cpp \
    model/setup.cpp \
    view/mainwindow.cpp \
    view/configuration.cpp

HEADERS  += model/rivermodel.h \
    model/status.h \
    model/cleanup.h \
    model/color.h \
    model/dump.h \
    model/globals.h \
    model/go.h \
    model/patch.h \
    model/pred.h \
    model/setup.h \
    view/mainwindow.h \
    view/configuration.h

FORMS    += view/mainwindow.ui
