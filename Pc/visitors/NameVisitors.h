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

std::string assertSimple(Context &c, Node *node);
std::string assertSimpleUnique(Context &c, Node *node);

class TypeNameExtractor : public Visitor
{
public:
    TypeNameExtractor(){ }

    std::string result() const { return r; }

    virtual void visit(TypeNode &node) override;
    virtual void visit(VarNode &node) override;

private:
    std::string r;
};

}

#endif // NAMEVISITORS_H
