#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/TypeBuilder.h"

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

    auto sym = sv.front();

    node.setProperty("sym", sym);

    if(!node.generics.empty())
    {
        std::vector<Type*> types;
        for(auto &g: node.generics)
        {
            types.push_back(TypeBuilder::build(c, g.get()));
        }

        node.setProperty("generics", types);

        c.genericRequests.insert(GenericRequest(sym, types));
    }
}

void ExprDecorator::visit(CallNode &node)
{
    node.target->accept(*this);

    for(auto &a: node.args)
    {
        a = decorate(c, a);
    }
}

NodePtr ExprDecorator::decorate(Context &c, NodePtr node)
{
    ExprDecorator ed(c);
    node->accept(ed);

    return ed.result() ? ed.result() : node;
}
