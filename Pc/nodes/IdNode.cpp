#include "IdNode.h"

#include "visitors/Visitor.h"

void IdNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr IdNode::cloneDetail() const
{
    auto node = new IdNode(location(), safeClone(parent), name);
    NodePtr n(node);

    node->params = safeClone(params);

    return n;
}
