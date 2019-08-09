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

HEADERS += \
    $$PWD/declare_metainfo.hpp \
    $$PWD/decorator.hpp \
    $$PWD/detail/compare.hpp \
    $$PWD/detail/json_converter.hpp \
    $$PWD/detail/member_count.hpp \
    $$PWD/detail/member_index.hpp \
    $$PWD/detail/member_name.hpp \
    $$PWD/detail/member_offset.hpp \
    $$PWD/detail/metaconfig.hpp \
    $$PWD/detail/metadata.hpp \
    $$PWD/detail/more_traits.hpp \
    $$PWD/metafunction.hpp \
    $$PWD/type_group.hpp

SOURCES += \
    $$PWD/detail/member_name.cpp


FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR/ -ljsoncpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR/ -ljsoncpp
else:unix: LIBS += -L$$DESTDIR/ -ljsoncpp

INCLUDEPATH += $$PWD/../thirdparty
DEPENDPATH += $$PWD/../thirdparty

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$DESTDIR/libjsoncpp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$DESTDIR/libjsoncpp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$DESTDIR/jsoncpp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$DESTDIR/jsoncpp.lib
else:unix: PRE_TARGETDEPS += $$DESTDIR/libjsoncpp.a
