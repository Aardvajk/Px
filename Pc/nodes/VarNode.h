#ifndef VARNODE_H
#define VARNODE_H

#include "nodes/Node.h"

class VarNode : public Node
{
public:
    explicit VarNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodePtr name;
    NodePtr type;
};

#endif // VARNODE_H
