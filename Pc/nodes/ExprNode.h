#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "nodes/Node.h"

class ExprNode : public Node
{
public:
    explicit ExprNode(Location location, NodePtr expr = { }) : Node(location), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual NodePtr cloneDetail() const override;

    NodePtr expr;
};

#endif // EXPRNODE_H
