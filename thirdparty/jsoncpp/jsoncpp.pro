#-------------------------------------------------
#
# Project created by QtCreator 2018-08-26T17:56:00
#
#-------------------------------------------------

QT -= gui

TARGET = jsoncpp
TEMPLATE = lib
DESTDIR = $$PWD/../..
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11 staticlib

MOC_DIR = ./tmp
UI_DIR = ./tmp
OBJECTS_DIR = ./tmp

HEADERS +=  \
    autolink.h \
    config.h \
    forwards.h \
    json.h \
    json_batchallocator.h \
    json_features.h \
    json_internalarray.inl \
    json_internalmap.inl \
    json_valueiterator.inl \
    reader.h \
    value.h \
    writer.h

SOURCES +=  \
    json_reader.cpp \
    json_value.cpp \
    json_writer.cpp


FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
