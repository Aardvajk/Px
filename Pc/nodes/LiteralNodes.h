#ifndef LITERALNODES_H
#define LITERALNODES_H

#include "nodes/Node.h"

class CharLiteralNode : public Node
{
public:
    CharLiteralNode(Location location, char value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    char value;
};

class IntLiteralNode : public Node
{
public:
    IntLiteralNode(Location location, int value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    int value;
};

#endif // LITERALNODES_H
