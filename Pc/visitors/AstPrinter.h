#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class AstPrinter : public Visitor
{
public:
    AstPrinter(Context &c, std::ostream &os);

    virtual void visit(BlockNode &node) override;

private:
    std::ostream &tab() const;

    Context &c;
    std::ostream &os;
    unsigned tc;
};

#endif // ASTPRINTER_H