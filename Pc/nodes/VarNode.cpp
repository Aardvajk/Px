#include "VarNode.h"

#include "visitors/Visitor.h"

void VarNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr VarNode::cloneDetail() const
{
    auto var = new VarNode(location(), safeClone(name));
    NodePtr n(var);

    var->type = safeClone(type);
    var->value = safeClone(value);

    return n;
}
