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

    NodePtr result() const { return rn; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;

    static NodePtr decorate(Context &c, NodePtr node, Type *expected = nullptr);

private:
    Context &c;
    Type *expected;
    NodePtr rn;
};

#endif // EXPRDECORATOR_H
