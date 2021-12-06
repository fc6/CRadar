#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CRadar
TEMPLATE = app
DESTDIR   =$${PWD}/bin


DEFINES += QT_DEPRECATED_WARNINGS



SOURCES += \
    src/main.cpp \
    src/widget.cpp \
    src/cradar.cpp

HEADERS += \
    src/cradar.h \
    src/widget.h
