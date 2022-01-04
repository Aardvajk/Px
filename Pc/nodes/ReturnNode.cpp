#include "ReturnNode.h"

#include "visitors/Visitor.h"

void ReturnNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr ReturnNode::cloneDetail() const
{
    auto node = new ReturnNode(location());
    NodePtr n(node);

    node->expr = safeClone(expr);

    return n;
}
