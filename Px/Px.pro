TEMPLATE = lib
CONFIG += console
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

INCLUDEPATH += "C:/Projects/pcx"

LIBS += "C:/Projects/pcx/build/release/libpcx.a"

SOURCES += \
    scanner/SourceList.cpp
    
HEADERS += \
    framework/Error.h \
    scanner/Location.h \
    scanner/SourceList.h
