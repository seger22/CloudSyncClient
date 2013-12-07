TEMPLATE = subdirs

SUBDIRS += \
    DBlib \
trayicon \
    LibInotify \
    LibChecksum \
    Client \
    Inotify \
    LibRabin \
    LibSha \
    FileHandler \
    ChunkHandler \
    BlockHandler \
    DeltaHandler \
Start \
ConfigurationManager \
    Zdelta



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lboost_thread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lboost_thread
else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -lboost_thread

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include


