#-------------------------------------------------
#
# Project created by QtCreator 2019-06-28T09:49:10
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QDesktop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mediaplayer.cpp \
    textticker.cpp \
    weather.cpp

HEADERS  += mainwindow.h \
    config.h \
    textticker.h \
    weather.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
