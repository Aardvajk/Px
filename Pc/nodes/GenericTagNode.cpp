#include "GenericTagNode.h"

#include "visitors/Visitor.h"

void GenericTagNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string GenericTagNode::classname() const
{
    return "generic tag";
}

NodePtr GenericTagNode::cloneDetail() const
{
    return new GenericTagNode(location(), name);
}
