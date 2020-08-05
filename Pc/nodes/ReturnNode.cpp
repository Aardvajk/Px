#include "ReturnNode.h"

#include "visitors/Visitor.h"

void ReturnNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr ReturnNode::cloneDetail() const
{
    return new ReturnNode(location(), safeClone(expr));
}
