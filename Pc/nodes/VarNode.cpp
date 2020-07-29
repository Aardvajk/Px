#include "VarNode.h"

#include "visitors/Visitor.h"

void VarNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string VarNode::classname() const
{
    return "var";
}

NodePtr VarNode::cloneDetail() const
{
    auto var = new VarNode(location(), safeClone(name));
    NodePtr n(var);

    var->type = safeClone(type);

    return n;
}
