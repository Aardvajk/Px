#ifndef EXPRDECORATOR_H
#define EXPRDECORATOR_H

#include "visitors/Visitor.h"

#include "nodes/Node.h"

class Context;
class Type;

class ExprDecorator : public Visitor
{
private:
    explicit ExprDecorator(Context &c, NodePtr &cn, Type *expected = nullptr);

public:
    NodePtr result() const { return rn; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(AddrNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(ThisNode &node) override;

    static NodePtr decorate(Context &c, NodePtr node, Type *expected = nullptr);

private:
    Context &c;
    NodePtr &cn;
    Type *expected;

    NodePtr rn;
};

#endif // EXPRDECORATOR_H
