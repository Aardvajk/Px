#include "LiteralNodes.h"

#include "visitors/Visitor.h"

void IntLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr IntLiteralNode::cloneDetail() const
{
    return new IntLiteralNode(location(), value);
}

void CharLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr CharLiteralNode::cloneDetail() const
{
    return new CharLiteralNode(location(), value);
}
