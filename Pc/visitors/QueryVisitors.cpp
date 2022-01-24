#include "QueryVisitors.h"

#include "nodes/Nodes.h"

QueryVisitors::GetParent::GetParent()
{
}

void QueryVisitors::GetParent::visit(IdNode &node)
{
    r = node.parent;
}
