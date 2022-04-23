#-------------------------------------------------
#
# Project created by QtCreator 2018-09-26T12:04:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCalculator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calculatorpushbutton.cpp \
    enterexitanimation.cpp

HEADERS  += mainwindow.h \
    calculatorpushbutton.h \
    config.h \
    enterexitanimation.h

RESOURCES += \
    resource.qrc

