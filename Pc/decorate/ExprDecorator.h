#ifndef EXPRDECORATOR_H
#define EXPRDECORATOR_H

#include "visitors/Visitor.h"

class Context;
class Type;
class Node;

class ExprDecorator : public Visitor
{
public:
    explicit ExprDecorator(Context &c, Type *expected = nullptr);

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;

private:
    Context &c;
    Type *expected;
};

#endif // EXPRDECORATOR_H
