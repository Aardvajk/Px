#include "VarNode.h"

#include "visitors/Visitor.h"

void VarNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr VarNode::cloneDetail() const
{
    auto node = new VarNode(location());
    NodePtr n(node);

    node->name = safeClone(name);
    node->type = safeClone(type);

    return n;
}
