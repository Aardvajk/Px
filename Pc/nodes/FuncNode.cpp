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

    func->type = safeClone(type);

    return n;
}
