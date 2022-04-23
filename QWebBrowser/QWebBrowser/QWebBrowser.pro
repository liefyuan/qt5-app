#-------------------------------------------------
#
# Project created by QtCreator 2019-08-05T12:15:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = QWebBrowser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    webpage.cpp \
    enterexitanimation.cpp

HEADERS  += mainwindow.h \
    webpage.h \
    enterexitanimation.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
