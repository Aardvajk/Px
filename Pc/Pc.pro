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

PRE_TARGETDEPS += "C:/Projects/Px/Px/build-Px/release/libPx.a"

SOURCES += \
        application/Context.cpp \
        main.cpp \
        nodes/BlockNode.cpp \
        nodes/FuncNode.cpp \
        nodes/IdNode.cpp \
        nodes/NamespaceNode.cpp \
        nodes/Node.cpp \
        nodes/ScopeNode.cpp \
        parser/CommonParser.cpp \
        parser/DeclarationParser.cpp \
        parser/Parser.cpp \
        syms/Sym.cpp \
        syms/SymGuard.cpp \
        syms/SymPrinter.cpp \
        syms/SymTree.cpp \
        visitors/AstPrinter.cpp \
        visitors/Describer.cpp \
        visitors/Visitor.cpp

HEADERS += \
    application/Context.h \
    nodes/BlockNode.h \
    nodes/FuncNode.h \
    nodes/IdNode.h \
    nodes/NamespaceNode.h \
    nodes/Node.h \
    nodes/Nodes.h \
    nodes/ScopeNode.h \
    parser/CommonParser.h \
    parser/DeclarationParser.h \
    parser/Parser.h \
    syms/Sym.h \
    syms/SymGuard.h \
    syms/SymPrinter.h \
    syms/SymTree.h \
    visitors/AstPrinter.h \
    visitors/Describer.h \
    visitors/Visitor.h

DISTFILES += \
    ../workspace/source.pc
