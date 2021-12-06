#ifndef DESCRIBER_H
#define DESCRIBER_H

#include "visitors/Visitor.h"

#include <string>
#include <sstream>

class Describer : public Visitor
{
public:
    Describer();

    std::string result() const { return r.str(); }

    virtual void visit(BlockNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(TypeNode &node) override;

private:
    std::ostringstream r;
};

#endif // DESCRIBER_H
