#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "nodes/Node.h"

class ClassNode : public Node
{
public:
    explicit ClassNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodePtr name;
    NodePtr body;
};

class TemplateClassNode : public Node
{
public:
    explicit TemplateClassNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodeList params;
    NodePtr name;
    NodePtr body;
    NodeList instances;
};

#endif // CLASSNODE_H
