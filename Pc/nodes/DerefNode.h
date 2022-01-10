#ifndef DEREFNODE_H
#define DEREFNODE_H

#include "nodes/Node.h"

class DerefNode : public Node
{
public:
    explicit DerefNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodePtr expr;
};

#endif // DEREFNODE_H
