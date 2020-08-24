#include "ClassNode.h"

#include "visitors/Visitor.h"

void ClassNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr ClassNode::cloneDetail() const
{
    auto node = new ClassNode(location(), safeClone(name));
    NodePtr n(node);

    node->body = safeClone(body);

    return n;
}
