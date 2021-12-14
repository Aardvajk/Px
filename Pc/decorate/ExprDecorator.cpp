#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/TypeQuery.h"

ExprDecorator::ExprDecorator(Context &c, Type *expected) : c(c), expected(expected)
{
}

void ExprDecorator::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
    }

    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);

    if(expected && expected->returnType)
    {
        std::vector<Sym*> matches;

        for(auto s: sv)
        {
            auto type = s->assertedProperty("type").to<Type*>();

            if(Type::compare(type->args, expected->args))
            {
                matches.push_back(s);
            }
        }

        sv = matches;
    }

    if(sv.empty())
    {
        throw Error(node.location(), "symbol not found - ", node.description());
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
