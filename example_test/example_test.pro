QT -= core gui

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/..
INCLUDEPATH += $$PWD/../thirdparty
MOC_DIR = ./tmp
UI_DIR = ./tmp
OBJECTS_DIR = ./tmp

SOURCES += \
        main.cpp

LIBS += -L$$PWD/.. -lmetainfo
