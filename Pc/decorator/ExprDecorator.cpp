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
            auto m = Match::create(c, expected, s->assertProperty("type").to<Type*>());
            if(m.valid && m.total() == expected->args.size())
            {
                map[m].push_back(s);
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

    node.setProperty("sym", sym);
}

void ExprDecorator::visit(CallNode &node)
{
    std::vector<Type*> args;
    for(auto &a: node.args)
    {
        decorate(c, a);
        args.push_back(TypeQuery::assert(c, a.get()));
    }

    auto type = c.types.insert(Type::function(c.types.nullType(), args));

    ExprDecorator::decorate(c, node.target, type);
}

void ExprDecorator::visit(CommaNode &node)
{
    ExprDecorator::decorate(c, node.left);
    ExprDecorator::decorate(c, node.right);
}

void ExprDecorator::decorate(Context &c, NodePtr node, Type *expected)
{
    ExprDecorator ed(c, expected);
    node->accept(ed);
}
