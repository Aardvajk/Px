#include "NamespaceNode.h"

#include "visitors/Visitor.h"

void NamespaceNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string NamespaceNode::classname() const
{
    return "namespace";
}

NodePtr NamespaceNode::cloneDetail() const
{
    auto ns = new NamespaceNode(location(), safeClone(name));
    NodePtr n(ns);

    ns->body = safeClone(body);

    return n;
}
