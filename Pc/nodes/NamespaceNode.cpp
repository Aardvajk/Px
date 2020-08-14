#include "NamespaceNode.h"

#include "visitors/Visitor.h"

void NamespaceNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr NamespaceNode::cloneDetail() const
{
    auto node = new NamespaceNode(location(), safeClone(name));
    NodePtr n(node);

    node->body = safeClone(body);

    return n;
}
