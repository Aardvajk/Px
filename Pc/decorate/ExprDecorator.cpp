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

ExprDecorator::ExprDecorator(Context &c, Type *expected) : c(c), expected(expected)
{
}

void ExprDecorator::visit(IdNode &node)
{
    if(node.parent)
    {
        Visitor::visit<ExprDecorator>(node.parent.get(), c);
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
            if(s->type() == Sym::Type::Func)
            {
                auto type = s->assertedProperty("type").to<Type*>();

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
    if(sym->type() == Sym::Type::Func)
    {
        for(auto &r: c.templateFuncReqs)
        {
            if(r.sym == sym)
            {
                ++r.uses;
            }
        }
    }

    node.setProperty("sym", sym);
}

void ExprDecorator::visit(CallNode &node)
{
    std::vector<Type*> args;

    for(auto a: node.args)
    {
        Visitor::visit<ExprDecorator>(a.get(), c);
        args.push_back(TypeQuery::assert(c, a.get()));
    }

    auto type = c.types.insert(Type::function(c.types.primitiveType(Primitive::Type::Null), args));

    Visitor::visit<ExprDecorator>(node.target.get(), c, type);
}

void ExprDecorator::visit(DerefNode &node)
{
    Visitor::visit<ExprDecorator>(node.expr.get(), c);

    if(!TypeQuery::assert(c, node.expr.get())->ptr)
    {
        throw Error(node.location(), "cannot deference a non-pointer - ", node.description());
    }
}

void ExprDecorator::visit(AddrNode &node)
{
    Visitor::visit<ExprDecorator>(node.expr.get(), c);
}

void ExprDecorator::visit(AssignNode &node)
{
    Visitor::visit<ExprDecorator>(node.target.get(), c);
    Visitor::visit<ExprDecorator>(node.value.get(), c);
}
