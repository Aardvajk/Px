#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "nodes/Node.h"

class BlockNode : public Node
{
public:
    explicit BlockNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;
    virtual NodePtr cloneDetail() const override;

    void push_back(Node *n);
    void push_back(NodePtr n);

    NodeList nodes;
};

#endif // BLOCKNODE_H
