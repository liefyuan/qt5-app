#-------------------------------------------------
#
# Project created by QtCreator 2018-12-13T16:09:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QNotepad
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vscroll.cpp \
    qfilechose.cpp \
    enterexitanimation.cpp

HEADERS  += mainwindow.h \
    config.h \
    vscroll.h \
    qfilechose.h \
    enterexitanimation.h

FORMS    += mainwindow.ui
include     ($$PWD/easyInput/easyInput.pri)
RESOURCES += \
    resource.qrc
