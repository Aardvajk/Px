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

std::string NameVisitors::assertSimple(Context &c, Node *node)
{
    if(!Visitor::query<IsNameSimple, bool>(node))
    {
        throw Error(node->location(), "symbol not found - ", node->description());
    }

    return Visitor::query<TrailingId, std::string>(node);
}

std::string NameVisitors::assertSimpleUnique(Context &c, Node *node)
{
    auto n = assertSimple(c, node);

    if(c.tree.current()->child(n))
    {
        throw Error(node->location(), "symbol already defined - ", n);
    }

    return n;
}

void NameVisitors::TypeNameExtractor::visit(TypeNode &node)
{
    if(Visitor::query<NameVisitors::IsNameSimple, bool>(node.name.get()))
    {
        r = Visitor::query<NameVisitors::TrailingId, std::string>(node.name.get());
    }
}

void NameVisitors::TypeNameExtractor::visit(VarNode &node)
{
    node.type->accept(*this);
}

