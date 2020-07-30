#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

ExprDecorator::ExprDecorator(Context &c) : c(c)
{
}


void ExprDecorator::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = decorate(c, node.parent);
    }

    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);

    if(sv.empty())
    {
        throw Error(node.location(), "symbol not found - ", node.description());
    }

    node.setProperty("sym", sv.front());
}

void ExprDecorator::visit(CallNode &node)
{
    node.target->accept(*this);
}

NodePtr ExprDecorator::decorate(Context &c, NodePtr node)
{
    ExprDecorator ed(c);
    node->accept(ed);

    return ed.result() ? ed.result() : node;
}
