#ifndef DECORATOR_H
#define DECORATOR_H

#include "visitors/Visitor.h"

class Context;

class Decorator : public Visitor
{
public:
    explicit Decorator(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(FuncNode &node) override;

    static void checkFunctionReturned(Context &c, FuncNode &node);

private:
    Context &c;
};

#endif // DECORATOR_H
