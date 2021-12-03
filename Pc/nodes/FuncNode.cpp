#include "FuncNode.h"

#include "visitors/Visitor.h"

void FuncNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr FuncNode::cloneDetail() const
{
    auto node = new FuncNode(location());
    NodePtr n(node);

    node->name = safeClone(name);
    node->body = safeClone(body);

    return n;
}
