#ifndef CALLNODE_H
#define CALLNODE_H

#include "nodes/Node.h"

class CallNode : public Node
{
public:
    explicit CallNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodePtr target;
    NodeList args;
};

#endif // CALLNODE_H
