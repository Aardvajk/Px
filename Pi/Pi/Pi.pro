TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

INCLUDEPATH += "C:/wclang/include" \
               "C:/Projects/pcx" \
               "C:/Projects/Px/Px/Px"

LIBS += "C:/Projects/Px/buildPx/release/libpx.a" \
        "C:/Projects/pcx/build/Release/libpcx.a"

PRE_TARGETDEPS += "C:/Projects/Px/buildPx/release/libPx.a"

SOURCES += \
        application/Context.cpp \
        main.cpp

HEADERS += \
    application/Context.h

DISTFILES += \
    ../../../workspace/source.pi
