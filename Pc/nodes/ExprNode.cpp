#include "ExprNode.h"

#include "visitors/Visitor.h"

void ExprNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr ExprNode::cloneDetail() const
{
    return new ExprNode(location(), safeClone(expr));
}
