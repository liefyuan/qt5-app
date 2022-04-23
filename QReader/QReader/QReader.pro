#-------------------------------------------------
#
# Project created by QtCreator 2018-12-20T18:44:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QReader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vscroll.cpp \
    enterexitanimation.cpp \
    qfilechose.cpp

HEADERS  += mainwindow.h \
    config.h \
    vscroll.h \
    enterexitanimation.h \
    qfilechose.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
