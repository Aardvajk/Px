#ifndef GLOBALINITGENERATOR_H
#define GLOBALINITGENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class GlobalInitGenerator : public Visitor
{
public:
    GlobalInitGenerator(Context &c, std::ostream &os);

    bool result() const { return ok; }

    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;

private:
    Context &c;
    std::ostream &os;
    bool ok;
};

#endif // GLOBALINITGENERATOR_H
