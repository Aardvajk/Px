#include "GenericNode.h"

#include "visitors/Visitor.h"

void GenericNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr GenericNode::cloneDetail() const
{
    return new GenericNode(location(), name);
}
