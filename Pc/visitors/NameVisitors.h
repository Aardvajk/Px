#ifndef NAMEVISITORS_H
#define NAMEVISITORS_H

#include "visitors/Visitor.h"

#include <string>

class Context;
class Node;

namespace NameVisitors
{

class IsNameSimple : public Visitor
{
public:
    IsNameSimple() : r(false) { }

    bool result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    bool r;
};

class TrailingId : public Visitor
{
public:
    TrailingId(){ }

    std::string result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    std::string r;
};

std::string assertSimpleUniqueName(Context &c, Node *node);

}

#endif // NAMEVISITORS_H
