#include "DerefNode.h"

#include "visitors/Visitor.h"

void DerefNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr DerefNode::cloneDetail() const
{
    auto node = new DerefNode(location());
    NodePtr n(node);

    node->expr = safeClone(expr);

    return n;
}
