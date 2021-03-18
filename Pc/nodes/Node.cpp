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
    return cloneDetail();
}

std::string Node::description() const
{
    return Visitor::query<DescVisitor, std::string>(const_cast<Node*>(this));
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
