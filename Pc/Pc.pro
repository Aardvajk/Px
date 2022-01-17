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
        decorate/ArgDecorator.cpp \
        decorate/Decorator.cpp \
        decorate/ExprDecorator.cpp \
        decorate/FuncDecorator.cpp \
        decorate/VarDecorator.cpp \
        finalise/Finaliser.cpp \
        generate/ExprGenerator.cpp \
        generate/FuncGenerator.cpp \
        generate/Generator.cpp \
        generate/LocalsGenerator.cpp \
        main.cpp \
        nodes/AddrNode.cpp \
        nodes/AssignNode.cpp \
        nodes/BlockNode.cpp \
        nodes/CallNode.cpp \
        nodes/ClassNode.cpp \
        nodes/DerefNode.cpp \
        nodes/ExprNode.cpp \
        nodes/FuncNode.cpp \
        nodes/IdNode.cpp \
        nodes/LiteralNodes.cpp \
        nodes/NamespaceNode.cpp \
        nodes/Node.cpp \
        nodes/ReturnNode.cpp \
        nodes/ScopeNode.cpp \
        nodes/TypeNode.cpp \
        nodes/VarNode.cpp \
        parser/CommonParser.cpp \
        parser/ExprParser.cpp \
        parser/FuncParser.cpp \
        parser/IncludeParser.cpp \
        parser/Parser.cpp \
        parser/TypeParser.cpp \
        syms/Sym.cpp \
        syms/SymFinder.cpp \
        syms/SymGuard.cpp \
        syms/SymPrinter.cpp \
        syms/SymScopeVisitor.cpp \
        syms/SymTree.cpp \
        templates/Templates.cpp \
        types/Type.cpp \
        types/TypeBuilder.cpp \
        types/TypeCache.cpp \
        types/TypeQuery.cpp \
        visitors/AstPrinter.cpp \
        visitors/Describer.cpp \
        visitors/NameVisitors.cpp \
        visitors/Visitor.cpp

HEADERS += \
    application/Context.h \
    components/FuncInfo.h \
    components/TemplateFuncReq.h \
    decorate/ArgDecorator.h \
    decorate/Decorator.h \
    decorate/ExprDecorator.h \
    decorate/FuncDecorator.h \
    decorate/VarDecorator.h \
    finalise/Finaliser.h \
    generate/ExprGenerator.h \
    generate/FuncGenerator.h \
    generate/Generator.h \
    generate/LocalsGenerator.h \
    nodes/AddrNode.h \
    nodes/AssignNode.h \
    nodes/BlockNode.h \
    nodes/CallNode.h \
    nodes/ClassNode.h \
    nodes/DerefNode.h \
    nodes/ExprNode.h \
    nodes/FuncNode.h \
    nodes/IdNode.h \
    nodes/LiteralNodes.h \
    nodes/NamespaceNode.h \
    nodes/Node.h \
    nodes/Nodes.h \
    nodes/ReturnNode.h \
    nodes/ScopeNode.h \
    nodes/TypeNode.h \
    nodes/VarNode.h \
    parser/CommonParser.h \
    parser/ExprParser.h \
    parser/FuncParser.h \
    parser/IncludeParser.h \
    parser/Parser.h \
    parser/TypeParser.h \
    syms/Sym.h \
    syms/SymFinder.h \
    syms/SymGuard.h \
    syms/SymPrinter.h \
    syms/SymScopeVisitor.h \
    syms/SymTree.h \
    templates/Templates.h \
    types/Type.h \
    types/TypeBuilder.h \
    types/TypeCache.h \
    types/TypeQuery.h \
    visitors/AstPrinter.h \
    visitors/Describer.h \
    visitors/NameVisitors.h \
    visitors/Visitor.h

DISTFILES += \
    ../unittests/basic.pc \
    ../unittests/basic_ptr.pc \
    ../unittests/deduce.pc \
    ../unittests/initialisers.pc \
    ../unittests/local_refs.pc \
    ../unittests/nested_template_class.pc \
    ../unittests/ptr_assign_deref.pc \
    ../unittests/return.pc \
    ../unittests/run.bat \
    ../unittests/runtest.bat \
    ../unittests/template_overload.pc \
    ../unittests/template_overloads.pc \
    ../unittests/templates.pc \
    ../workspace/run.bat \
    ../workspace/source.pc \
    ../workspace/source.pi
