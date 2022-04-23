#-------------------------------------------------
#
# Project created by QtCreator 2018-12-14T15:43:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QLed
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    enterexitanimation.cpp

HEADERS  += mainwindow.h \
    config.h \
    enterexitanimation.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
