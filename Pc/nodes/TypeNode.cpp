#include "TypeNode.h"

#include "visitors/Visitor.h"

void TypeNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr TypeNode::cloneDetail() const
{
    auto node = new TypeNode(location(), safeClone(name));
    NodePtr n(node);

    node->ref = ref;
    node->ptr = ptr;

    return n;
}
