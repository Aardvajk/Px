#include "AddrNode.h"

#include "visitors/Visitor.h"

void AddrNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr AddrNode::cloneDetail() const
{
    auto node = new AddrNode(location());
    NodePtr n(node);

    node->expr = safeClone(expr);

    return n;
}
