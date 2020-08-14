#include "CallNode.h"

#include "visitors/Visitor.h"

void CallNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr CallNode::cloneDetail() const
{
    auto node = new CallNode(location(), safeClone(target));
    NodePtr n(node);

    node->args = safeClone(args);

    return n;
}
