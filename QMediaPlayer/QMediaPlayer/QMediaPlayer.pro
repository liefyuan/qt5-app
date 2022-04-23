#-------------------------------------------------
#
# Project created by QtCreator 2019-06-14T15:56:38
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMediaPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mediaplayer.cpp \
    eventfilter.cpp \
    enterexitanimation.cpp \
    qfilechose.cpp

HEADERS  += mainwindow.h \
    config.h \
    enterexitanimation.h \
    qfilechose.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
