#include "CallNode.h"

#include "visitors/Visitor.h"

void CallNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr CallNode::cloneDetail() const
{
    auto call = new CallNode(location(), safeClone(target));
    NodePtr n(call);

    call->args = safeClone(args);

    return n;
}
