#ifndef THISNODE_H
#define THISNODE_H

#include "nodes/Node.h"

class ThisNode : public Node
{
public:
    ThisNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;
};

#endif // THISNODE_H
