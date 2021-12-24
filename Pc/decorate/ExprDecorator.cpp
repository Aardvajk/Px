#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include "decorate/Decorator.h"
#include "decorate/ArgDecorator.h"
#include "decorate/VarDecorator.h"

#include <pcx/scoped_push.h>
#include <pcx/str.h>
#include <pcx/join_str.h>

namespace
{

Sym *generateTemplateFuncSym(Context &c, Sym *sym, IdNode &id)
{
    auto node = sym->assertedProperty("node").to<TemplateFuncNode*>();
    auto pv = sym->property("params").to<std::vector<std::string> >();

    std::unordered_map<std::string, Type*> map;
    for(std::size_t i = 0; i < pv.size(); ++i)
    {
        map[pv[i]] = TypeQuery::assert(c, id.params[i].get());
    }

    auto g = pcx::scoped_push(c.templateParams, map);

    std::vector<Type*> args;

    NodeList copyArgs;
    for(auto a: node->args)
    {
        copyArgs.push_back(a->clone());
    }

    for(auto a: copyArgs)
    {
        Visitor::visit<ArgDecorator>(a.get(), c);
        args.push_back(TypeQuery::assert(c, a.get()));
    }

    auto returnType = c.types.primitiveType(Primitive::Type::Null);

    if(node->type)
    {
        auto copy = node->type->clone();
        returnType = TypeBuilder::build(c, copy.get());
    }

    auto type = c.types.insert(Type::function(returnType, args));

    auto name = pcx::str(sym->name(), "<", pcx::join_str(args, ",", [](const Type *t){ return t->description(); }), ">(", pcx::join_str(node->args, ","), "):", returnType->description());

    if(auto s = sym->parent()->child(name))
    {
        return s;
    }

    auto s = sym->parent()->add(new Sym(Sym::Type::Func, node->location(), name));

    s->setProperty("type", type);

    if(node->body)
    {
        auto fn = new FuncNode(node->location());
        node->instances.push_back(fn);

        auto tn = new TypeNode(node->location(), new IdNode(node->location(), { }, { }));
        fn->type = tn;

        tn->setProperty("type", returnType);

        fn->name = new IdNode(node->location(), { }, name);

        NodeList args;
        for(auto a: node->args)
        {
            args.push_back(a->clone());
        }

        fn->args = args;
        fn->body = node->body->clone();

        fn->setProperty("sym", s);

        Visitor::visit<Decorator>(fn, c);
    }

    return s;
}

}

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
            s = generateTemplateFuncSym(c, s, node);
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
