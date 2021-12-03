#include "Node.h"

#include "framework/Error.h"

#include "visitors/Describer.h"

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
    return Visitor::query<Describer, std::string>(const_cast<Node*>(this));
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

pcx::any Node::assertedProperty(const std::string &name) const
{
    auto a = property(name);
    if(!a)
    {
        throw Error(location(), "Node::assertProperty failed - ", name, " ", description());
    }

    return a;
}

NodePtr Node::safeClone(const NodePtr &n)
{
    return n ? n->clone() : NodePtr();
}

NodeList Node::safeClone(const NodeList &n)
{
    NodeList r;
    for(auto &i: n)
    {
        r.push_back(safeClone(i));
    }

    return r;
}
