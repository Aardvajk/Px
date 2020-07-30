#include "CallNode.h"

#include "visitors/Visitor.h"

void CallNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string CallNode::classname() const
{
    return "call";
}

NodePtr CallNode::cloneDetail() const
{
    return new CallNode(location(), safeClone(target));
}
