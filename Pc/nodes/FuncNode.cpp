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
    node->args = safeClone(args);
    node->type = safeClone(type);
    node->body = safeClone(body);

    return n;
}

void TemplateFuncNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr TemplateFuncNode::cloneDetail() const
{
    auto node = new TemplateFuncNode(location());
    NodePtr n(node);

    node->params = safeClone(params);
    node->name = safeClone(name);
    node->args = safeClone(args);
    node->type = safeClone(type);
    node->body = safeClone(body);
    node->instances = safeClone(instances);

    return n;
}
