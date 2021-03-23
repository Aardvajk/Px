#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class AstPrinter : public Visitor
{
public:
    AstPrinter(Context &c, std::ostream &os);

    virtual void visit(BlockNode &node);
    virtual void visit(IdNode &node);
    virtual void visit(NamespaceNode &node);
    virtual void visit(FuncNode &node);
    virtual void visit(ClassNode &node);
    virtual void visit(TypeNode &node);
    virtual void visit(ScopeNode &node);

private:
    std::ostream &tab() const;

    Context &c;
    std::ostream &os;

    unsigned tc;
};

#endif // ASTPRINTER_H
