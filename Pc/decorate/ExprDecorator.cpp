#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include "templates/Templates.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

#include <unordered_set>
#include <algorithm>

ExprDecorator::ExprDecorator(Context &c, NodePtr &cn, Type *expected) : c(c), cn(cn), expected(expected)
{
}

void ExprDecorator::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprDecorator::decorate(c, node.parent);

        auto t = TypeQuery::query(c, node.parent.get());
        if(t && t->ptr)
        {
            throw Error(node.location(), "cannot access via pointer - ", node.description());
        }
    }

    for(auto p: node.params)
    {
        p->setProperty("type", TypeBuilder::build(c, p.get()));
    }

    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);

    for(auto &s: sv)
    {
        if(s->type() == Sym::Type::TemplateFunc)
        {
            if(!expected || !expected->returnType)
            {
                throw Error(node.location(), "cannot instantiate template func without call - ", node.description());
            }

            auto type = s->assertedProperty("type").to<Type*>();

            if(Type::compareNonTemplates(c, type->args, expected->args))
            {
                s = Templates::generateFuncReq(c, s, expected, node);
            }
        }
    }

    if(expected && expected->returnType)
    {
        class Match
        {
        public:
            Match(Sym *sym, std::size_t conversions) : sym(sym), conversions(conversions) { }

            Sym *sym;
            std::size_t conversions;
        };

        std::vector<Match> matches;

        for(auto s: sv)
        {
            if(auto t = s->property("type"))
            {
                auto type = t.to<Type*>();

                if(type->returnType)
                {
                    if(type->args.size() == expected->args.size())
                    {
                        auto ps = s->property("paramindices").value<std::unordered_set<std::size_t> >();

                        std::size_t m = 0;
                        std::size_t cv = 0;

                        for(std::size_t i = 0; i < type->args.size(); ++i)
                        {
                            if(Type::compare(type->args[i], expected->args[i]))
                            {
                                ++m;
                                if(ps.find(i) != ps.end())
                                {
                                    ++cv;
                                }
                            }
                        }

                        if(m == type->args.size())
                        {
                            matches.push_back(Match(s, cv));
                        }
                    }
                }
            }
        }

        std::vector<Sym*> least;
        if(!matches.empty())
        {
            std::sort(matches.begin(), matches.end(), [](Match a, Match b){ return a.conversions < b.conversions; });

            auto min = matches.front().conversions;

            std::size_t i = 0;
            auto v = min;

            while(i < matches.size() && v == min)
            {
                least.push_back(matches[i].sym);
                v = matches[++i].conversions;
            }
        }

        sv = least;
    }

    if(sv.empty())
    {
        throw Error(node.location(), "symbol not found - ", node.description(), expected ? pcx::str("(", pcx::join_str(expected->args, ","), ")") : "");
    }

    if(sv.size() > 1)
    {
        throw Error(node.location(), "ambiguous symbol - ", node.description());
    }

    auto sym = sv.front();

    if(auto t = sym->property("type"))
    {
        if(t.to<Type*>()->ref)
        {
            auto d = new DerefNode(node.location());
            rn = d;

            d->expr = cn;
            cn = rn;
        }
    }

    if(sym->type() == Sym::Type::Func)
    {
        for(auto &r: c.templateFuncReqs)
        {
            if(r.sym == sym)
            {
                r.used = true;
            }
        }
    }

    node.setProperty("sym", sym);

    if(sym->property("method").value<bool>() || sym->property("member").value<bool>())
    {
        if(!node.parent)
        {
            auto t = new ThisNode(node.location());
            node.parent = t;

            decorate(c, node.parent);
        }
    }
}

void ExprDecorator::visit(CallNode &node)
{
    std::vector<Type*> args;

    for(auto &a: node.args)
    {
        a = ExprDecorator::decorate(c, a);
        args.push_back(TypeQuery::assert(c, a.get()));
    }

    auto type = c.types.insert(Type::function(c.types.nullType(), args));

    node.target = ExprDecorator::decorate(c, node.target, type);

    type = TypeQuery::assertCallable(c, node.target.get());

    for(std::size_t i = 0; i < type->args.size(); ++i)
    {
        if(type->args[i]->ref)
        {
            auto a = new AddrNode(node.location());
            a->expr = node.args[i];

            node.args[i] = a;
        }
    }

    if(type->returnType->ref)
    {
        auto n = new DerefNode(node.location());
        n->expr = cn;

        rn = n;
    }
}

void ExprDecorator::visit(DerefNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);

    if(!TypeQuery::assert(c, node.expr.get())->ptr)
    {
        throw Error(node.location(), "cannot deference a non-pointer - ", node.description());
    }
}

void ExprDecorator::visit(AddrNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);
}

void ExprDecorator::visit(AssignNode &node)
{
    node.target = ExprDecorator::decorate(c, node.target);
    node.value = ExprDecorator::decorate(c, node.value);

    auto tt = TypeQuery::assert(c, node.target.get());
    auto vt = TypeQuery::assert(c, node.value.get());

    if(!Type::compare(tt, vt))
    {
        throw Error(node.location(), "type mismatche in assignment - ", tt->description(), " expected");
    }
}

void ExprDecorator::visit(ThisNode &node)
{
    auto s = c.tree.current()->container();

    if(!s->assertedProperty("type").to<Type*>()->method)
    {
        throw Error(node.location(), "this cannot be used outside method - ", node.description());
    }

    auto t = Type::primary(s->parent());
    t.ref = true;

    node.setProperty("type", c.types.insert(t));
}

NodePtr ExprDecorator::decorate(Context &c, NodePtr node, Type *expected)
{
    ExprDecorator el(c, node, expected);
    node->accept(el);

    return el.result() ? el.result() : node;
}
