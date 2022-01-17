#ifndef VARNODE_H
#define VARNODE_H

#include "nodes/Node.h"

class VarNode : public Node
{
public:
    explicit VarNode(Location location) : Node(location), ref(false) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    bool ref;

    NodePtr name;
    NodePtr type;
    NodePtr value;
};

#endif // VARNODE_H
