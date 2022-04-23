#-------------------------------------------------
#
# Project created by QtCreator 2019-06-28T09:49:10
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QOnlineWeather
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    weather.cpp \
    enterexitanimation.cpp

HEADERS  += mainwindow.h \
    weater.h \
    enterexitanimation.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
