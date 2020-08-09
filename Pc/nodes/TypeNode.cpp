#include "TypeNode.h"

#include "visitors/Visitor.h"

void TypeNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr TypeNode::cloneDetail() const
{
    auto type = new TypeNode(location());
    NodePtr n(type);

    type->name = safeClone(name);
    type->args = safeClone(args);
    type->returnType = safeClone(returnType);

    return n;
}
