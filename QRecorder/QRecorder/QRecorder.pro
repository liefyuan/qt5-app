#-------------------------------------------------
#
# Project created by QtCreator 2019-04-22T17:53:16
#
#-------------------------------------------------

QT       += core gui \
            multimedia \
            multimediawidgets \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRecorder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mediaplyer.cpp \
    evenfilter.cpp \
    enterexitanimation.cpp

HEADERS  += mainwindow.h \
    config.h \
    enterexitanimation.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
