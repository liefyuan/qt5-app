#-------------------------------------------------
#
# Project created by QtCreator 2019-12-24T18:37:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCamera
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    enterexitanimation.cpp \
    rollingbox.cpp

HEADERS  += mainwindow.h \
    enterexitanimation.h \
    rollingbox.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
