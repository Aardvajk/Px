#include "ScopeNode.h"

#include "visitors/Visitor.h"

void ScopeNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr ScopeNode::cloneDetail() const
{
    auto node = new ScopeNode(location());
    NodePtr n(node);

    node->body = safeClone(body);

    return n;
}
