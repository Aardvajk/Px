#ifndef EXPRDECORATOR_H
#define EXPRDECORATOR_H

#include "visitors/Visitor.h"

#include "nodes/Node.h"

class Context;

class ExprDecorator : public Visitor
{
public:
    explicit ExprDecorator(Context &c);

    NodePtr result() const { return rn; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;

    static NodePtr decorate(Context &c, NodePtr node);

private:
    Context &c;
    NodePtr rn;
};

#endif // EXPRDECORATOR_H
