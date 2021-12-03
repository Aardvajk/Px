#include "NamespaceNode.h"

#include "visitors/Visitor.h"

void NamespaceNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr NamespaceNode::cloneDetail() const
{
    auto node = new NamespaceNode(location());
    NodePtr n(node);

    node->name = safeClone(name);
    node->body = safeClone(body);

    return n;
}
