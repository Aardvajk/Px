#ifndef IDNODE_H
#define IDNODE_H

#include "nodes/Node.h"

class IdNode : public Node
{
public:
    IdNode(Location location, NodePtr parent, std::string name) : Node(location), parent(parent), name(std::move(name)) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodePtr parent;
    std::string name;
    NodeList params;
};

#endif // IDNODE_H
