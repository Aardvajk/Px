#ifndef GENERICTAGNODE_H
#define GENERICTAGNODE_H

#include "nodes/Node.h"

class GenericTagNode : public Node
{
public:
    GenericTagNode(Location location, std::string name) : Node(location), name(std::move(name)) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    std::string name;
};

#endif // GENERICTAGNODE_H
