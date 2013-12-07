#-------------------------------------------------
#
# Project created by QtCreator 2013-10-25T19:26:09
#
#-------------------------------------------------

QT       -= gui
QT +=sql

TARGET = DBlib
TEMPLATE = lib

DEFINES += DBLIB_LIBRARY

SOURCES += dblib.cpp

HEADERS += dblib.h\
        dblib_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
