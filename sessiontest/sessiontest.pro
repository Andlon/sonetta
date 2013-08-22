QT       += core

QT       -= gui

TARGET = sessiontest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += "$$PWD/libspotify/include"

win32:LIBS += -L$$PWD/libspotify/lib -llibspotify
unix:LIBS += -L$$PWD/libspotify/lib -lspotify

RCC_DIR = "build/rcc"
UI_DIR = "build/uic"
MOC_DIR = "build/moc"
OBJECTS_DIR = "build/obj"

CONFIG(debug, debug|release) {
    DESTDIR = "debug"
}
CONFIG(release, debug|release) {
    DESTDIR = "release"
}

SOURCES += main.cpp \
    sessiontest.cpp

HEADERS += sessiontest.h
