#include "IdNode.h"

#include "visitors/Visitor.h"

void IdNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string IdNode::classname() const
{
    return "id";
}

NodePtr IdNode::cloneDetail() const
{
    return new IdNode(location(), safeClone(parent), name);
}
