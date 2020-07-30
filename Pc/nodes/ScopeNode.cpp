#include "ScopeNode.h"

#include "visitors/Visitor.h"

void ScopeNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ScopeNode::classname() const
{
    return "scope";
}

NodePtr ScopeNode::cloneDetail() const
{
    auto scope = new ScopeNode(location());
    NodePtr n(scope);

    scope->body = safeClone(body);

    return n;
}
