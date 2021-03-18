#ifndef DESCVISITOR_H
#define DESCVISITOR_H

#include "visitors/Visitor.h"

#include <string>

class DescVisitor : public Visitor
{
public:
    std::string result() const { return r; }

    virtual void visit(BlockNode &node) override;
    virtual void visit(IdNode &node) override;

private:
    std::string r;
};

#endif // DESCVISITOR_H
