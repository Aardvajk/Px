#include "Node.h"

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
