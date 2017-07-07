#-------------------------------------------------
#
# Project created by QtCreator 2016-03-06T14:20:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EP-App
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    epc.cpp \
    info.cpp

HEADERS  += widget.h \
    epc.h \
    incfile.h \
    info.h

FORMS    += widget.ui

DISTFILES += \
    Notes.txt
