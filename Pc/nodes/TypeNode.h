#ifndef TYPENODE_H
#define TYPENODE_H

#include "nodes/Node.h"

class TypeNode : public Node
{
public:
    explicit TypeNode(Location location) : Node(location), function(false) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    bool function;

    NodePtr name;
    NodeList generics;
    NodeList args;
    NodePtr returnType;
};

#endif // TYPENODE_H
