#include "ExprNode.h"

#include "visitors/Visitor.h"

void ExprNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr ExprNode::cloneDetail() const
{
    auto node = new ExprNode(location());
    NodePtr n(node);

    node->expr = safeClone(expr);

    return n;
}
