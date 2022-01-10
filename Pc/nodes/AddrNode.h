#ifndef ADDRNODE_H
#define ADDRNODE_H

#include "nodes/Node.h"

class AddrNode : public Node
{
public:
    explicit AddrNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodePtr expr;
};

#endif // ADDRNODE_H
