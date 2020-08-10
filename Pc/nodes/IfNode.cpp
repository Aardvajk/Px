#include "IfNode.h"

#include "visitors/Visitor.h"

void IfNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr IfNode::cloneDetail() const
{
    auto node = new IfNode(location());
    NodePtr n(node);

    node->expr = safeClone(expr);
    node->body = safeClone(body);
    node->elseBody = safeClone(elseBody);

    return n;
}
