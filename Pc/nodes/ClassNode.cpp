#include "ClassNode.h"

#include "visitors/Visitor.h"

void ClassNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr ClassNode::cloneDetail() const
{
    auto node = new ClassNode(location());
    NodePtr n(node);

    node->name = safeClone(name);
    node->body = safeClone(body);

    return n;
}

void TemplateClassNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr TemplateClassNode::cloneDetail() const
{
    auto node = new TemplateClassNode(location());
    NodePtr n(node);

    node->params = safeClone(params);
    node->name = safeClone(name);
    node->body = safeClone(body);
    node->instances = safeClone(instances);

    return n;
}
