#-------------------------------------------------
#
# Project created by QtCreator 2019-06-25T08:56:43
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fileWatcher
TEMPLATE = app
include(qxtglobalshortcut5-master/qxt.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    smtp.cpp \
    adb_find.cpp \
    sendthread.cpp \
    xmlutils.cpp

HEADERS  += mainwindow.h \
    smtp.h \
    adb_find.h \
    sendthread.h \
    xmlutils.h \
    constlist.h

FORMS    += mainwindow.ui
