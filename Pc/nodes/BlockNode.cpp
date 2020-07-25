#include "BlockNode.h"

#include "visitors/Visitor.h"

void BlockNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string BlockNode::classname() const
{
    return "block";
}