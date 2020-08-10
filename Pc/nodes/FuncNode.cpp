#include "FuncNode.h"

#include "visitors/Visitor.h"

void FuncNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr FuncNode::cloneDetail() const
{
    auto func = new FuncNode(location(), safeClone(name));
    NodePtr n(func);

    func->genericTags = safeClone(genericTags);
    func->args = safeClone(args);
    func->returnType = safeClone(returnType);
    func->body = safeClone(body);

    return n;
}
