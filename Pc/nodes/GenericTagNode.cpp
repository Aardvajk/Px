#include "GenericTagNode.h"

#include "visitors/Visitor.h"

void GenericTagNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr GenericTagNode::cloneDetail() const
{
    return new GenericTagNode(location(), name);
}
