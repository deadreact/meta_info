QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/..

MOC_DIR = ./tmp
UI_DIR = ./tmp
OBJECTS_DIR = ./tmp

SOURCES += \
        main.cpp

LIBS += -L$$PWD/.. -lmetainfo
