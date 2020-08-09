#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include "components/Match.h"

#include <pcx/indexed_range.h>
#include <pcx/optional.h>

#include <map>

namespace
{

bool isCandidate(const IdNode &node, const Sym *candidate)
{
    if(!node.generics.empty())
    {
        if(!candidate->property("generics"))
        {
            return false;
        }
    }

    return true;
}

pcx::optional<std::size_t> locateGenericArg(const std::vector<Type*> &args, std::size_t index)
{
    for(auto a: pcx::indexed_range(args))
    {
        if(a.value->gref && a.value->gref->index == index)
        {
            return a.index;
        }
    }

    return { };
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
        std::map<Match, std::vector<Sym*> > map;

        for(auto s: sv)
        {
            if(isCandidate(node, s))
            {
                auto m = Match::create(expected, s->assertProperty("type").to<Type*>());
                if(m.total() == expected->args.size())
                {
                    map[m].push_back(s);
                }
            }
        }

        sv.clear();
        if(!map.empty())
        {
            for(auto s: map.rbegin()->second)
            {
                sv.push_back(s);
            }
        }
    }

    if(sv.empty())
    {
        throw Error(node.location(), "symbol not found - ", node.description(), expected ? expected->description() : "");
    }

    if(sv.size() > 1)
    {
        throw Error(node.location(), "ambiguous - ", node.description(), expected ? expected->description() : "");
    }

    auto sym = sv.front();

    if(!node.generics.empty() && sym->type() != Sym::Type::Func)
    {
        throw Error(node.location(), "invalid generics - ", node.description());
    }

    node.setProperty("sym", sym);

    std::vector<Type*> deduced;
    if(auto p = sym->property("generics"))
    {
        auto candidate = p.to<GenericParamList>();
        auto type = sym->assertProperty("type").to<Type*>();

        while(node.generics.size() + deduced.size() < candidate.size())
        {
            auto index = node.generics.size() + deduced.size();

            auto found = locateGenericArg(type->args, index);
            if(!found)
            {
                throw Error(node.location(), "cannot deduce generic - ", candidate[index].name);
            }

            deduced.push_back(expected->args[*found]);
        }
    }

    if(!node.generics.empty() || !deduced.empty())
    {
        std::vector<Type*> types;
        for(auto &g: node.generics)
        {
            auto type = c.generics.updateType(TypeBuilder::build(c, g.get()));
            types.push_back(type);
        }

        for(auto t: deduced)
        {
            types.push_back(t);
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
