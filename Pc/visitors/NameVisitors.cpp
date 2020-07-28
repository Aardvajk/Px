#include "NameVisitors.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

void NameVisitors::IsNameSimple::visit(IdNode &node)
{
    r = !node.parent;
}

void NameVisitors::TrailingId::visit(IdNode &node)
{
    r = node.name;
}

std::string NameVisitors::assertSimpleUniqueName(Context &c, Node *node)
{
    if(!Visitor::query<IsNameSimple, bool>(node))
    {
        throw Error(node->location(), "simple name expected - ", node->description());
    }

    auto n = Visitor::query<TrailingId, std::string>(node);
    if(c.tree.current()->child(n))
    {
        throw Error(node->location(), "symbol already defined - ", n);
    }

    return n;
}
