#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T09:34:04
#
#-------------------------------------------------

QT       += core

QT       += gui

TARGET = CBIR
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    imagehandler.cpp \
    histogrambase.cpp \
    histogramrgb.cpp \
    calculatefeature.cpp

HEADERS += \
    imagehandler.h \
    histogrambase.h \
    histogramrgb.h \
    calculatefeature.h \
    calculationrsp.h
