TEMPLATE = lib
CONFIG += console
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

INCLUDEPATH += "C:/Projects/pcx"

LIBS += "C:/Projects/pcx/build/release/libpcx.a"

SOURCES += \
    scanner/Lexer.cpp \
    scanner/Scanner.cpp \
    scanner/Source.cpp \
    scanner/SourceList.cpp \
    scanner/Token.cpp
    
HEADERS += \
    framework/Error.h \
    scanner/Lexer.h \
    scanner/Location.h \
    scanner/Scanner.h \
    scanner/Source.h \
    scanner/SourceList.h \
    scanner/Token.h
