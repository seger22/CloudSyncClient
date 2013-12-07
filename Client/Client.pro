#-------------------------------------------------
#
# Project created by QtCreator 2013-06-10T22:02:06
#
#-------------------------------------------------

QT       += core
QT += sql

QT       -= gui

TARGET = Client


TEMPLATE = lib

SOURCES += \
    client.cpp \
    clienthandler.cpp

HEADERS += \
    chunkdat.h \
    client.h \
    clienthandler.h \
    blockChecksumSerial.h \
    chunkhash.h

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


<<<<<<< HEAD


=======
>>>>>>> 34197256fb4986b0d90cb96dcde07edd565aa4ee
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibChecksum/release/ -lLibChecksum
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibChecksum/debug/ -lLibChecksum
else:unix: LIBS += -L$$OUT_PWD/../LibChecksum/ -lLibChecksum

INCLUDEPATH += $$PWD/../LibChecksum
DEPENDPATH += $$PWD/../LibChecksum

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ChunkHandler/release/ -lChunkHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ChunkHandler/debug/ -lChunkHandler
else:unix: LIBS += -L$$OUT_PWD/../ChunkHandler/ -lChunkHandler

INCLUDEPATH += $$PWD/../ChunkHandler
DEPENDPATH += $$PWD/../ChunkHandler

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ConfigurationManager/release/ -lConfigurationManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ConfigurationManager/debug/ -lConfigurationManager
else:unix: LIBS += -L$$OUT_PWD/../ConfigurationManager/ -lConfigurationManager

INCLUDEPATH += $$PWD/../ConfigurationManager
DEPENDPATH += $$PWD/../ConfigurationManager

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DBlib/release/ -lDBlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DBlib/debug/ -lDBlib
else:unix: LIBS += -L$$OUT_PWD/../DBlib/ -lDBlib

INCLUDEPATH += $$PWD/../DBlib
DEPENDPATH += $$PWD/../DBlib
