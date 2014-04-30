#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T22:49:58
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = quickglobalstatemachinetest
CONFIG   += console c++11
CONFIG   -= app_bundle

RCC_DIR = "build/rcc"
UI_DIR = "build/uic"
MOC_DIR = "build/moc"
OBJECTS_DIR = "build/obj"

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../src/quick/quickglobalstatemachine.h

SOURCES += quickglobalstatemachinetest.cpp \
    ../../src/quick/quickglobalstatemachine.cpp



