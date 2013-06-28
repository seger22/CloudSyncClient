#-------------------------------------------------
#
# Project created by QtCreator 2013-05-28T13:25:10
#
#-------------------------------------------------

QT       -= gui

TARGET = LibInotify
TEMPLATE = lib

DEFINES += LIBINOTIFY_LIBRARY

SOURCES +=inotify-cxx.cpp

HEADERS +=libinotify_global.h \
    inotify-cxx.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
