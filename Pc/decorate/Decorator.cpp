#include "Decorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include "visitors/NameVisitors.h"

#include "decorate/ArgDecorator.h"
#include "decorate/VarDecorator.h"
#include "decorate/FuncDecorator.h"

#include <pcx/scoped_push.h>

namespace
{

Sym *search(Context &c, Sym::Type type, Node *node)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != type)
        {
            throw Error(node->location(), Sym::toString(type), " expected - ", s->fullname());
        }

        return s;
    }

    return nullptr;
}

Sym *searchFunc(Context &c, Node *node, Type *func)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func && s->type() != Sym::Type::TemplateFunc)
        {
            throw Error(node->location(), "function expected - ", s->fullname());
        }

        auto t = s->assertedProperty("type").to<Type*>();

        if(Type::compare(func->args, t->args))
        {
            if(!Type::compare(func->returnType, t->returnType))
            {
                throw Error(node->location(), "mismatched return type - ", s->fullname());
            }

            return s;
        }
    }

    return nullptr;
}

}

Decorator::Decorator(Context &c) : c(c)
{
}

void Decorator::visit(BlockNode &node)
{
    for(auto n: node.nodes)
    {
        n->accept(*this);
    }
}

void Decorator::visit(NamespaceNode &node)
{
    auto sym = search(c, Sym::Type::Namespace, node.name.get());
    if(!sym)
    {
        auto name = NameVisitors::assertSimpleUnique(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Namespace, node.location(), name));
    }

    node.setProperty("sym", sym);

    auto g = c.tree.open(sym);
    node.body->accept(*this);
}


void Decorator::visit(FuncNode &node)
{
    Sym *sym = nullptr;

    if(auto s = node.property("sym"))
    {
        sym = s.to<Sym*>();
    }

    if(!sym)
    {
        std::vector<Type*> args;
        for(auto a: node.args)
        {
            Visitor::visit<ArgDecorator>(a.get(), c);
            args.push_back(TypeQuery::assert(c, a.get()));
        }

        auto returnType = c.types.primitiveType(Primitive::Type::Null);

        if(node.type)
        {
            returnType = TypeBuilder::build(c, node.type.get());
        }

        if(returnType->ref)
        {
            returnType = c.types.insert(returnType->addPointer());
        }

        auto type = c.types.insert(Type::function(returnType, args));

        sym = searchFunc(c, node.name.get(), type);
        if(!sym)
        {
            auto name = NameVisitors::assertSimple(c, node.name.get());
            sym = c.tree.current()->add(new Sym(Sym::Type::Func, node.location(), name));

            sym->setProperty("type", type);
        }

        node.setProperty("sym", sym);
    }

    if(node.body)
    {
        if(sym->property("defined").value<bool>())
        {
            throw Error(node.location(), "function already defined - ", node.name->description());
        }

        sym->setProperty("defined", true);

        c.funcInfos.push_back(new FuncInfo());
        sym->setProperty("info", c.funcInfos.back_ptr());

        auto g = c.tree.open(sym);

        for(auto &a: node.args)
        {
            Visitor::visit<VarDecorator>(a.get(), c);
        }

        Visitor::visit<FuncDecorator>(node.body.get(), c);
    }
}

void Decorator::visit(TemplateFuncNode &node)
{
    std::vector<std::string> params;
    for(auto p: node.params)
    {
        params.push_back(Visitor::query<NameVisitors::TrailingId, std::string>(p.get()));
    }

    std::unordered_map<std::string, Type*> map;
    for(auto p: params)
    {
        map[p] = c.types.unknownTemplateType();
    }

    std::vector<Type*> args;
    Type *returnType = c.types.primitiveType(Primitive::Type::Null);

    if(true)
    {
        auto g = pcx::scoped_push(c.templateParams, map);

        NodeList copyArgs;
        for(auto a: node.args)
        {
            copyArgs.push_back(a->clone());
        }

        for(auto a: copyArgs)
        {
            Visitor::visit<ArgDecorator>(a.get(), c);
            args.push_back(TypeQuery::assert(c, a.get()));
        }

        if(node.type)
        {
            auto copy = node.type->clone();
            returnType = TypeBuilder::build(c, copy.get());
        }
    }

    auto type = c.types.insert(Type::function(returnType, args));

    auto sym = searchFunc(c, node.name.get(), type);
    if(!sym)
    {
        auto name = NameVisitors::assertSimple(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::TemplateFunc, node.location(), name));

        sym->setProperty("type", type);
    }

    node.setProperty("sym", sym);

    sym->setProperty("params", params);
    sym->setProperty("node", &node);

    if(node.body)
    {
        if(sym->property("defined").value<bool>())
        {
            throw Error(node.location(), "template function already defined - ", node.name->description());
        }

        sym->setProperty("defined", true);
        sym->setProperty("body", node.body.get());
    }
}

void Decorator::visit(VarNode &node)
{
    Visitor::visit<VarDecorator>(&node, c);
}

void Decorator::visit(ClassNode &node)
{
    Sym *sym = nullptr;

    if(auto s = node.property("sym"))
    {
        sym = s.to<Sym*>();
    }
    else
    {
        sym = search(c, Sym::Type::Class, node.name.get());
        if(!sym)
        {
            auto name = NameVisitors::assertSimpleUnique(c, node.name.get());
            sym = c.tree.current()->add(new Sym(Sym::Type::Class, node.location(), name));
        }

        node.setProperty("sym", sym);
    }

    if(node.body)
    {
        if(sym->property("defined").value<bool>())
        {
            throw Error(node.location(), "class already defined - ", node.name->description());
        }

        sym->setProperty("defined", true);

        auto g = c.tree.open(sym);
        node.body->accept(*this);
    }
}

void Decorator::visit(TemplateClassNode &node)
{
    auto sym = search(c, Sym::Type::TemplateClass, node.name.get());
    if(!sym)
    {
        auto name = NameVisitors::assertSimpleUnique(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::TemplateClass, node.location(), name));
    }

    node.setProperty("sym", sym);

    std::vector<std::string> params;
    for(auto p: node.params)
    {
        params.push_back(Visitor::query<NameVisitors::TrailingId, std::string>(p.get()));
    }

    sym->setProperty("params", params);
    sym->setProperty("node", &node);

    if(node.body)
    {
        if(sym->property("defined").value<bool>())
        {
            throw Error(node.location(), "class already defined - ", node.name->description());
        }

        sym->setProperty("defined", true);
    }
}
