#ifndef ASSIGNNODE_H
#define ASSIGNNODE_H

#include "nodes/Node.h"

class AssignNode : public Node
{
public:
    AssignNode(Location location, NodePtr target) : Node(location), target(target) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodePtr target;
    NodePtr value;
};

#endif // ASSIGNNODE_H
