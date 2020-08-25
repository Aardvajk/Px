#ifndef GENERICNODE_H
#define GENERICNODE_H

#include "nodes/Node.h"

class GenericNode : public Node
{
public:
    GenericNode(Location location, std::string name) : Node(location), name(std::move(name)) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    std::string name;
};

#endif // GENERICNODE_H
