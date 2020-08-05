#include "FuncNode.h"

#include "visitors/Visitor.h"

void FuncNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string FuncNode::classname() const
{
    return "func";
}

NodePtr FuncNode::cloneDetail() const
{
    auto func = new FuncNode(location(), safeClone(name));
    NodePtr n(func);

    func->genericTags = safeClone(genericTags);
    func->args = safeClone(args);
    func->type = safeClone(type);
    func->body = safeClone(body);

    return n;
}
