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
        components/FuncInfo.cpp \
        components/Match.cpp \
        decorator/ArgDecorator.cpp \
        decorator/Decorator.cpp \
        decorator/ExprDecorator.cpp \
        decorator/FuncDecorator.cpp \
        decorator/VarDecorator.cpp \
        generator/CommonGenerator.cpp \
        generator/ExprGenerator.cpp \
        generator/FuncGenerator.cpp \
        generator/Generator.cpp \
        generator/LocalsGenerator.cpp \
        generics/GenericRequestSet.cpp \
        generics/GenericStack.cpp \
        generics/Generics.cpp \
        main.cpp \
        nodes/BlockNode.cpp \
        nodes/CallNode.cpp \
        nodes/ExprNode.cpp \
        nodes/FuncNode.cpp \
        nodes/GenericTagNode.cpp \
        nodes/IdNode.cpp \
        nodes/IfNode.cpp \
        nodes/LiteralNodes.cpp \
        nodes/NamespaceNode.cpp \
        nodes/Node.cpp \
        nodes/ReturnNode.cpp \
        nodes/ScopeNode.cpp \
        nodes/TypeNode.cpp \
        nodes/VarNode.cpp \
        parser/CommonParser.cpp \
        parser/DeclarationParser.cpp \
        parser/DirectiveParser.cpp \
        parser/ExprParser.cpp \
        parser/FuncParser.cpp \
        parser/Parser.cpp \
        parser/TypeParser.cpp \
        syms/Sym.cpp \
        syms/SymFinder.cpp \
        syms/SymGuard.cpp \
        syms/SymPrinter.cpp \
        syms/SymScopeVisitor.cpp \
        syms/SymTree.cpp \
        types/PrimitiveTypes.cpp \
        types/Type.cpp \
        types/TypeBuilder.cpp \
        types/TypeCache.cpp \
        types/TypeQuery.cpp \
        visitors/AstPrinter.cpp \
        visitors/DescVisitor.cpp \
        visitors/NameVisitors.cpp \
        visitors/Visitor.cpp

HEADERS += \
    application/Context.h \
    components/Match.h \
    decorator/ArgDecorator.h \
    decorator/Decorator.h \
    decorator/ExprDecorator.h \
    decorator/FuncDecorator.h \
    decorator/VarDecorator.h \
    generator/CommonGenerator.h \
    generator/ExprGenerator.h \
    generator/FuncGenerator.h \
    generator/Generator.h \
    generator/LocalsGenerator.h \
    generics/GenericRef.h \
    generics/GenericRequest.h \
    generics/GenericRequestSet.h \
    generics/GenericStack.h \
    generics/Generics.h \
    components/FuncInfo.h \
    components/ParseInfo.h \
    nodes/BlockNode.h \
    nodes/CallNode.h \
    nodes/ExprNode.h \
    nodes/FuncNode.h \
    nodes/GenericTagNode.h \
    nodes/IdNode.h \
    nodes/IfNode.h \
    nodes/LiteralNodes.h \
    nodes/NamespaceNode.h \
    nodes/Node.h \
    nodes/Nodes.h \
    nodes/ReturnNode.h \
    nodes/ScopeNode.h \
    nodes/TypeNode.h \
    nodes/VarNode.h \
    parser/CommonParser.h \
    parser/DeclarationParser.h \
    parser/DirectiveParser.h \
    parser/ExprParser.h \
    parser/FuncParser.h \
    parser/Parser.h \
    parser/TypeParser.h \
    syms/Sym.h \
    syms/SymFinder.h \
    syms/SymGuard.h \
    syms/SymPrinter.h \
    syms/SymScopeVisitor.h \
    syms/SymTree.h \
    types/PrimitiveTypes.h \
    types/Type.h \
    types/TypeBuilder.h \
    types/TypeCache.h \
    types/TypeQuery.h \
    visitors/AstPrinter.h \
    visitors/DescVisitor.h \
    visitors/NameVisitors.h \
    visitors/Visitor.h

DISTFILES += \
    ../workspace/source.pc \
    ../workspace/stdtest.ph \
    ../workspace/stdtest.pi
