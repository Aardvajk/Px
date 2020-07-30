#include "LiteralNodes.h"

#include "visitors/Visitor.h"

void CharLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string CharLiteralNode::classname() const
{
    return "charliteral";
}

NodePtr CharLiteralNode::cloneDetail() const
{
    return new CharLiteralNode(location(), value);
}

void IntLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string IntLiteralNode::classname() const
{
    return "intliteral";
}

NodePtr IntLiteralNode::cloneDetail() const
{
    return new IntLiteralNode(location(), value);
}
