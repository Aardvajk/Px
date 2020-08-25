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

    node->generics = safeClone(generics);
    node->args = safeClone(args);
    node->returnType = safeClone(returnType);
    node->body = safeClone(body);

    return n;
}
