#-------------------------------------------------
#
# Project created by QtCreator 2013-06-10T22:02:06
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Client


TEMPLATE = lib

SOURCES += \
    client.cpp \
    clienthandler.cpp \
    chunkhandler.cpp \
    chunkdat.cpp \
    filehandler.cpp

HEADERS += \
    client.h \
    clienthandler.h \
    chunkhandler.h \
    chunkdat.h \
    filehandler.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/release/ -lboost_system
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/debug/ -lboost_system
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/ -lboost_system -pthread

INCLUDEPATH += $$PWD/../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/release/ -lboost_thread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/debug/ -lboost_thread
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/ -lboost_thread

INCLUDEPATH += $$PWD/../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/release/ -lboost_serialization
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/debug/ -lboost_serialization
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/ -lboost_serialization

INCLUDEPATH += $$PWD/../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../usr/include




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibRabin/release/ -lLibRabin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibRabin/debug/ -lLibRabin
else:unix: LIBS += -L$$OUT_PWD/../LibRabin/ -lLibRabin

INCLUDEPATH += $$PWD/../LibRabin
DEPENDPATH += $$PWD/../LibRabin

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibSha/release/ -lLibSha
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibSha/debug/ -lLibSha
else:unix: LIBS += -L$$OUT_PWD/../LibSha/ -lLibSha

INCLUDEPATH += $$PWD/../LibSha
DEPENDPATH += $$PWD/../LibSha
