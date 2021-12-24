#ifndef FUNCNODE_H
#define FUNCNODE_H

#include "nodes/Node.h"

class FuncNode : public Node
{
public:
    explicit FuncNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodePtr name;
    NodeList args;
    NodePtr type;
    NodePtr body;
};

class TemplateFuncNode : public Node
{
public:
    explicit TemplateFuncNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodeList params;
    NodePtr name;
    NodeList args;
    NodePtr type;
    NodePtr body;
    NodeList instances;
};

#endif // FUNCNODE_H
