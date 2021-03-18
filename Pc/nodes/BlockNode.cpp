#include "BlockNode.h"

#include "visitors/Visitor.h"

void BlockNode::accept(Visitor &v)
{
    v.visit(*this);
}

NodePtr BlockNode::cloneDetail() const
{
    auto node = new BlockNode(location());
    NodePtr n(node);

    for(auto i: nodes)
    {
        node->nodes.push_back(safeClone(i));
    }

    return n;
}

void BlockNode::push_back(Node *n)
{
    nodes.push_back(n);
}

void BlockNode::push_back(NodePtr n)
{
    nodes.push_back(n);
}
