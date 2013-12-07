#-------------------------------------------------
#
# Project created by QtCreator 2013-10-16T00:54:01
#
#-------------------------------------------------

QT       -= gui

TARGET = DeltaHandler
TEMPLATE = lib

DEFINES += DELTAHANDLER_LIBRARY

SOURCES += deltahandler.cpp

HEADERS += deltahandler.h\
        deltahandler_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Zdelta/release/ -lZdelta
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Zdelta/debug/ -lZdelta
else:unix: LIBS += -L$$OUT_PWD/../Zdelta/ -lZdelta

INCLUDEPATH += $$PWD/../Zdelta
DEPENDPATH += $$PWD/../Zdelta
