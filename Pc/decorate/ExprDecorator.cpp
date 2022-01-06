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
        std::vector<Sym*> matches;

        for(auto s: sv)
        {
            if(s->type() == Sym::Type::Func)
            {
                auto type = s->assertedProperty("type").to<Type*>();

                if(Type::compare(type->args, expected->args))
                {
                    matches.push_back(s);
                }
            }
        }

        sv = matches;
    }

    if(sv.empty())
    {
        throw Error(node.location(), "symbol not found - ", node.description(), expected ? pcx::str("(", pcx::join_str(expected->args, ","), ")") : "");
    }

    if(sv.size() > 1)
    {
        throw Error(node.location(), "ambiguous symbol - ", node.description());
    }

    node.setProperty("sym", sv.front());
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
