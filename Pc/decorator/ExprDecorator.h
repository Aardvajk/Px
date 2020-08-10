#ifndef EXPRDECORATOR_H
#define EXPRDECORATOR_H

#include "visitors/Visitor.h"

#include "nodes/Node.h"

class Context;
class Type;

class ExprDecorator : public Visitor
{
public:
    ExprDecorator(Context &c, Type *expected);

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(CommaNode &node) override;

    static void decorate(Context &c, NodePtr node, Type *expected = nullptr);

private:
    Context &c;
    Type *expected;
};

#endif // EXPRDECORATOR_H
