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

    node->ref = ref;

    node->name = safeClone(name);
    node->type = safeClone(type);
    node->value = safeClone(value);

    return n;
}
