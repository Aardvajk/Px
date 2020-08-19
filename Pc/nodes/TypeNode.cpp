#include "TypeNode.h"

#include "visitors/Visitor.h"

void TypeNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr TypeNode::cloneDetail() const
{
    auto node = new TypeNode(location());
    NodePtr n(node);

    node->name = safeClone(name);
    node->args = safeClone(args);
    node->returnType = safeClone(returnType);

    return n;
}
