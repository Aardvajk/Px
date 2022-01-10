#include "AssignNode.h"

#include "visitors/Visitor.h"

void AssignNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr AssignNode::cloneDetail() const
{
    auto node = new AssignNode(location(), safeClone(target));
    NodePtr n(node);

    node->value = safeClone(value);

    return n;
}
