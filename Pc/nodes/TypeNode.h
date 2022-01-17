#ifndef TYPENODE_H
#define TYPENODE_H

#include "nodes/Node.h"

class TypeNode : public Node
{
public:
    explicit TypeNode(Location location, NodePtr name) : Node(location), ref(false), ptr(0), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    bool ref;
    std::size_t ptr;
    NodePtr name;
};

#endif // TYPENODE_H
