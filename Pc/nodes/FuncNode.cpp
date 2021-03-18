#include "FuncNode.h"

#include "visitors/Visitor.h"

void FuncNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr FuncNode::cloneDetail() const
{
    auto node = new FuncNode(location(), safeClone(name));
    NodePtr n(node);

    node->body = safeClone(body);

    return n;
}
