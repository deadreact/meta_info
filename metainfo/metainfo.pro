#-------------------------------------------------
#
# Project created by QtCreator 2018-08-26T17:56:00
#
#-------------------------------------------------

QT -= gui

TARGET = metainfo
TEMPLATE = lib
DESTDIR = $$PWD/..
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11 staticlib

MOC_DIR = ./tmp
UI_DIR = ./tmp
OBJECTS_DIR = ./tmp

SOURCES += \
        $$PWD/detail/member_name.cpp

HEADERS += \
    type_group.hpp \
    $$PWD/detail/member_offset.hpp \
    $$PWD/detail/json_converter.hpp \
    $$PWD/detail/member_count.hpp \
    $$PWD/detail/member_name.hpp \
    $$PWD/detail/compare.hpp \
    $$PWD/detail/more_traits.hpp \
    declare_types_as_tuple.hpp \
    metafunction.hpp

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
