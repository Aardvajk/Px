TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

QMAKE_CXXFLAGS += -std=gnu++11

QMAKE_LFLAGS += -Wl,--exclude-all-symbols

QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas -Wno-comment -Wno-maybe-uninitialized -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-attributes

INCLUDEPATH += "C:/Projects/pcx" \
               "C:/Projects/Px/Px/Px"

LIBS += "C:/Projects/pcx/build-pcx/release/libpcx.a" \
        "C:/Projects/Px/Px/build-Px/release/libPx.a"

PRE_TARGETDEPS += "C:/Projects/Px/Px/build-Px/release/libPx.a" \
                  "C:/Projects/Px/Px/build-Pi/release/Pi.exe" \
                  "C:/Projects/Px/Px/build-Pd/release/Pd.exe" \
                  "C:/Projects/Px/Px/build-Pl/release/Pl.exe" \
                  "C:/Projects/Px/Px/build-Pv/release/Pv.exe"

SOURCES += \
        main.cpp

DISTFILES += \
    ../workspace/pb.bat