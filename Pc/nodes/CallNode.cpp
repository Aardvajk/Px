#include "CallNode.h"

#include "visitors/Visitor.h"

void CallNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr CallNode::cloneDetail() const
{
    return new CallNode(location(), safeClone(target));
}
