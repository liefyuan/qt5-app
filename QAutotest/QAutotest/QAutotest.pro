#-------------------------------------------------
#
# Project created by QtCreator 2019-11-19T10:16:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QAutotest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vscroll.cpp \
    enterexitanimation.cpp

HEADERS  += mainwindow.h \
    vscroll.h \
    config.h \
    enterexitanimation.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
