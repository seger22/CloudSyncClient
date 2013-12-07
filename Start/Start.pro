QT +=sql

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
CONFIG += sql


SOURCES += \
    main.cpp
LIBS+=-L/usr/lib -lboost_system -lboost_filesystem -pthread


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DBlib/release/ -lDBlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DBlib/debug/ -lDBlib
else:unix: LIBS += -L$$OUT_PWD/../DBlib/ -lDBlib

INCLUDEPATH += $$PWD/../DBlib
DEPENDPATH += $$PWD/../DBlib


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibInotify/release/ -lLibInotify
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibInotify/debug/ -lLibInotify
else:unix: LIBS += -L$$OUT_PWD/../LibInotify/ -lLibInotify

INCLUDEPATH += $$PWD/../LibInotify
DEPENDPATH += $$PWD/../LibInotify

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Inotify/release/ -lInotify
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Inotify/debug/ -lInotify
else:unix: LIBS += -L$$OUT_PWD/../Inotify/ -lInotify

INCLUDEPATH += $$PWD/../Inotify
DEPENDPATH += $$PWD/../Inotify



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lboost_thread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lboost_thread
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lboost_thread

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include

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


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibSha/release/ -lLibSha
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibSha/debug/ -lLibSha
else:unix: LIBS += -L$$OUT_PWD/../LibSha/ -lLibSha

INCLUDEPATH += $$PWD/../LibSha
DEPENDPATH += $$PWD/../LibSha


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibChecksum/release/ -lLibChecksum
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibChecksum/debug/ -lLibChecksum
else:unix: LIBS += -L$$OUT_PWD/../LibChecksum/ -lLibChecksum

INCLUDEPATH += $$PWD/../LibChecksum
DEPENDPATH += $$PWD/../LibChecksum

<<<<<<< HEAD
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FileHandler/release/ -lFileHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FileHandler/debug/ -lFileHandler
else:unix: LIBS += -L$$OUT_PWD/../FileHandler/ -lFileHandler

INCLUDEPATH += $$PWD/../FileHandler
DEPENDPATH += $$PWD/../FileHandler

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ChunkHandler/release/ -lChunkHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ChunkHandler/debug/ -lChunkHandler
else:unix: LIBS += -L$$OUT_PWD/../ChunkHandler/ -lChunkHandler

INCLUDEPATH += $$PWD/../ChunkHandler
DEPENDPATH += $$PWD/../ChunkHandler

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

andler

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../BlockHandler/release/ -lBlockHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../BlockHandler/debug/ -lBlockHandler
else:unix: LIBS += -L$$OUT_PWD/../BlockHandler/ -lBlockHandler

INCLUDEPATH += $$PWD/../BlockHandler
DEPENDPATH += $$PWD/../BlockHandler


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
=======
>>>>>>> 34197256fb4986b0d90cb96dcde07edd565aa4ee
