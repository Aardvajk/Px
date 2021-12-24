#include "Templates.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include "decorate/Decorator.h"
#include "decorate/ArgDecorator.h"

#include "visitors/Visitor.h"

#include <pcx/str.h>
#include <pcx/join_str.h>
#include <pcx/scoped_push.h>

Sym *Templates::generateClass(Context &c, Sym *sym, NodeList &params)
{
    for(auto p: params)
    {
        p->setProperty("type", TypeBuilder::build(c, p.get()));
    }

    auto name = pcx::str(sym->fullname(), "<", pcx::join_str(params, ",", [](const NodePtr &n){ return n->property("type").to<Type*>()->description(); }), ">");

    if(auto s = sym->parent()->child(name))
    {
        return s;
    }

    auto node = sym->property("node").to<TemplateClassNode*>();
    auto pv = sym->property("params").to<std::vector<std::string> >();

    auto s = sym->parent()->add(new Sym(Sym::Type::Class, node->location(), name));

    auto cn = new ClassNode(node->location());

    cn->name = new IdNode(node->location(), { }, name);
    cn->body = node->body->clone();

    cn->setProperty("sym", s);

    if(pv.size() != params.size())
    {
        throw Error(sym->location(), "mismatched template params - ", sym->fullname());
    }

    std::unordered_map<std::string, Type*> map;
    for(std::size_t i = 0; i < pv.size(); ++i)
    {
        map[pv[i]] = TypeQuery::assert(c, params[i].get());
    }

    auto g = pcx::scoped_push(c.templateParams, map);

    Visitor::visit<Decorator>(cn, c);

    node->instances.push_back(cn);

    return s;
}

Sym *Templates::generateFunc(Context &c, Sym *sym, IdNode &id)
{
    auto node = sym->assertedProperty("node").to<TemplateFuncNode*>();
    auto pv = sym->property("params").to<std::vector<std::string> >();

    if(pv.size() != id.params.size())
    {
        throw Error(sym->location(), "mismatched template params - ", sym->fullname());
    }

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
