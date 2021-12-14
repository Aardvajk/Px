#include "CallNode.h"

#include "visitors/Visitor.h"

void CallNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr CallNode::cloneDetail() const
{
    auto node = new CallNode(location());
    NodePtr n(node);

    node->target = safeClone(target);
    node->args = safeClone(args);

    return n;
}
