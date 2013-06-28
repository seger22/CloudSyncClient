#-------------------------------------------------
#
# Project created by QtCreator 2013-05-28T13:25:40
#
#-------------------------------------------------

QT       -= gui

TARGET = Inotify
TEMPLATE = lib

DEFINES += INOTIFY_LIBRARY

SOURCES +=inotifyhandler.cpp \
    eventchecker.cpp \
    util.cpp \
    cmd.cpp

HEADERS +=inotify_global.h \
    inotifyhandler.h \
    eventchecker.h \
    util.h \
    cmd.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


LIBS+=-L/usr/lib -lboost_system -lboost_filesystem


INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibInotify/release/ -lLibInotify
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibInotify/debug/ -lLibInotify
else:unix: LIBS += -L$$OUT_PWD/../LibInotify/ -lLibInotify

INCLUDEPATH += $$PWD/../LibInotify
DEPENDPATH += $$PWD/../LibInotify





win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Client/release/ -lClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Client/debug/ -lClient
else:unix: LIBS += -L$$OUT_PWD/../Client/ -lClient

INCLUDEPATH += $$PWD/../Client
DEPENDPATH += $$PWD/../Client

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibRabin/release/ -lLibRabin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibRabin/debug/ -lLibRabin
else:unix: LIBS += -L$$OUT_PWD/../LibRabin/ -lLibRabin

INCLUDEPATH += $$PWD/../LibRabin
DEPENDPATH += $$PWD/../LibRabin
