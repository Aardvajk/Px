#include "CommaNode.h"

#include "visitors/Visitor.h"

void CommaNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr CommaNode::cloneDetail() const
{
    return new CommaNode(location(), safeClone(left), safeClone(right));
}
