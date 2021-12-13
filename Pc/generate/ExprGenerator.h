#ifndef EXPRGENERATOR_H
#define EXPRGENERATOR_H

#include "visitors/Visitor.h"

#include <pcx/optional.h>

#include <iostream>

class Context;
class Node;

class ExprGenerator : public Visitor
{
public:
    ExprGenerator(Context &c, std::ostream &os);

    pcx::optional<std::size_t> result() const { return r; }

    virtual void visit(IdNode &node) override;

    static std::size_t generate(Context &c, std::ostream &os, Node *node);

private:
    Context &c;
    std::ostream &os;

    pcx::optional<std::size_t> r;
};

#endif // EXPRGENERATOR_H