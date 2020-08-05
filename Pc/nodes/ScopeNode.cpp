#include "ScopeNode.h"

#include "visitors/Visitor.h"

void ScopeNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr ScopeNode::cloneDetail() const
{
    auto scope = new ScopeNode(location());
    NodePtr n(scope);

    scope->body = safeClone(body);

    return n;
}
