#include "IdNode.h"

#include "visitors/Visitor.h"

void IdNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr IdNode::cloneDetail() const
{
    auto id = new IdNode(location(), safeClone(parent), name);
    NodePtr n(id);

    id->generics = safeClone(generics);

    return n;
}
