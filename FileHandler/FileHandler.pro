#-------------------------------------------------
#
# Project created by QtCreator 2013-10-05T02:14:14
#
#-------------------------------------------------

QT       -= gui
QT +=sql

TARGET = FileHandler
TEMPLATE = lib

DEFINES += FILEHANDLER_LIBRARY

SOURCES += filehandler.cpp

HEADERS += filehandler.h\
        filehandler_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ChunkHandler/release/ -lChunkHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ChunkHandler/debug/ -lChunkHandler
else:unix: LIBS += -L$$OUT_PWD/../ChunkHandler/ -lChunkHandler

INCLUDEPATH += $$PWD/../ChunkHandler
DEPENDPATH += $$PWD/../ChunkHandler

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibRabin/release/ -lLibRabin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibRabin/debug/ -lLibRabin
else:unix: LIBS += -L$$OUT_PWD/../LibRabin/ -lLibRabin

INCLUDEPATH += $$PWD/../LibRabin
DEPENDPATH += $$PWD/../LibRabin

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibChecksum/release/ -lLibChecksum
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibChecksum/debug/ -lLibChecksum
else:unix: LIBS += -L$$OUT_PWD/../LibChecksum/ -lLibChecksum

INCLUDEPATH += $$PWD/../LibChecksum
DEPENDPATH += $$PWD/../LibChecksum

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Client/release/ -lClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Client/debug/ -lClient
else:unix: LIBS += -L$$OUT_PWD/../Client/ -lClient

INCLUDEPATH += $$PWD/../Client
DEPENDPATH += $$PWD/../Client

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DeltaHandler/release/ -lDeltaHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DeltaHandler/debug/ -lDeltaHandler
else:unix: LIBS += -L$$OUT_PWD/../DeltaHandler/ -lDeltaHandler

INCLUDEPATH += $$PWD/../DeltaHandler
DEPENDPATH += $$PWD/../DeltaHandler

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Zdelta/release/ -lZdelta
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Zdelta/debug/ -lZdelta
else:unix: LIBS += -L$$OUT_PWD/../Zdelta/ -lZdelta

INCLUDEPATH += $$PWD/../Zdelta
DEPENDPATH += $$PWD/../Zdelta

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../BlockHandler/release/ -lBlockHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../BlockHandler/debug/ -lBlockHandler
else:unix: LIBS += -L$$OUT_PWD/../BlockHandler/ -lBlockHandler

INCLUDEPATH += $$PWD/../BlockHandler
DEPENDPATH += $$PWD/../BlockHandler

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lboost_thread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lboost_thread
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lboost_thread

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DBlib/release/ -lDBlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DBlib/debug/ -lDBlib
else:unix: LIBS += -L$$OUT_PWD/../DBlib/ -lDBlib

INCLUDEPATH += $$PWD/../DBlib
DEPENDPATH += $$PWD/../DBlib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ConfigurationManager/release/ -lConfigurationManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ConfigurationManager/debug/ -lConfigurationManager
else:unix: LIBS += -L$$OUT_PWD/../ConfigurationManager/ -lConfigurationManager

INCLUDEPATH += $$PWD/../ConfigurationManager
DEPENDPATH += $$PWD/../ConfigurationManager
