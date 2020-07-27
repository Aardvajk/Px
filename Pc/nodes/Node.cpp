#include "Node.h"

#include "visitors/DescVisitor.h"

Node::Node(Location location) : n(location)
{
}

Node::~Node()
{
}

NodePtr Node::clone() const
{
    auto n = cloneDetail();

    n->pm = pm;
    return n;
}

void Node::setProperty(const std::string &name, pcx::any value)
{
    pm[name] = value;
}

std::string Node::description() const
{
    return Visitor::query<DescVisitor, std::string>(const_cast<Node*>(this));
}

pcx::any Node::property(const std::string &name) const
{
    auto i = pm.find(name);
    if(i != pm.end())
    {
        return i->second;
    }

    return { };
}

NodePtr Node::safeClone(const NodePtr &n)
{
    return n ? n->clone() : NodePtr();
}
