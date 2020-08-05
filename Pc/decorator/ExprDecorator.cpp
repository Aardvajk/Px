#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

namespace
{

bool compatible(const Type *expected, const Type *candidate)
{
    if(candidate->gref)
    {
        return true;
    }

    return Type::exact(expected, candidate);
}

bool compatible(const std::vector<Type*> &expected, const std::vector<Type*> &candidate)
{
    if(expected.size() != candidate.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < expected.size(); ++i)
    {
        if(!compatible(expected[i], candidate[i]))
        {
            return false;
        }
    }

    return true;
}

}

ExprDecorator::ExprDecorator(Context &c, Type *expected) : c(c), expected(expected)
{
}

void ExprDecorator::visit(IdNode &node)
{
    if(node.parent)
    {
        decorate(c, node.parent);
    }

    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);

    if(expected && expected->function())
    {
        std::vector<Sym*> v;
        for(auto s: sv)
        {
            if(compatible(expected->args, s->assertProperty("type").to<Type*>()->args))
            {
                v.push_back(s);
            }
        }

        sv = v;
    }

    if(sv.empty())
    {
        throw Error(node.location(), "symbol not found - ", node.description());
    }

    if(sv.size() > 1)
    {
        throw Error(node.location(), "ambiguous - ", node.description());
    }

    auto sym = sv.front();

    node.setProperty("sym", sym);

    if(!node.generics.empty())
    {
        std::vector<Type*> types;
        for(auto &g: node.generics)
        {
            auto type = c.generics.updateType(TypeBuilder::build(c, g.get()));
            types.push_back(type);
        }

        node.setProperty("generics", types);

        c.genericRequests.insert(GenericRequest(sym, types));
    }
}

void ExprDecorator::visit(CallNode &node)
{
    std::vector<Type*> args;
    for(auto &a: node.args)
    {
        decorate(c, a);
        args.push_back(c.generics.updateType(TypeQuery::assert(c, a.get())));
    }

    auto type = c.types.insert(Type::function(c.types.nullType(), args));

    ExprDecorator::decorate(c, node.target, type);
}

void ExprDecorator::decorate(Context &c, NodePtr node, Type *expected)
{
    ExprDecorator ed(c, expected);
    node->accept(ed);
}
