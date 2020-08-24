#include "Decorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "visitors/NameVisitors.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include "decorator/ArgDecorator.h"
#include "decorator/FuncDecorator.h"
#include "decorator/VarDecorator.h"

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

Sym *searchFunction(Context &c, Node *node, Type *type)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func)
        {
            throw Error(node->location(), "function expected - ", s->fullname());
        }

        if(Type::exact(type->args, s->property("type").to<Type*>()->args))
        {
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
    for(auto &n: node.nodes)
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
    std::vector<Type*> args;
    for(auto &a: node.args)
    {
        Visitor::visit<ArgDecorator>(a.get(), c);
        args.push_back(TypeQuery::assert(c, a.get()));
    }

    auto returnType = c.types.nullType();

    if(node.returnType)
    {
        returnType = TypeBuilder::build(c, node.returnType.get());
        node.returnType->setProperty("type", returnType);
    }

    auto type = c.types.insert(Type::function(returnType, args));

    auto sym = searchFunction(c, node.name.get(), type);
    if(sym)
    {
        if(!Type::exact(sym->property("type").to<Type*>()->returnType, type->returnType))
        {
            throw Error(node.location(), "mismatched return type - ", node.description());
        }
    }
    else
    {
        auto name = NameVisitors::assertSimple(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Func, node.location(), name));

        sym->setProperty("type", type);
    }

    node.setProperty("sym", sym);

    if(node.body)
    {
        if(sym->property("defined"))
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
        checkFunctionReturned(c, node);
    }
}

void Decorator::visit(VarNode &node)
{
    Visitor::visit<VarDecorator>(&node, c);
}

void Decorator::visit(ClassNode &node)
{
    auto sym = search(c, Sym::Type::Class, node.name.get());
    if(!sym)
    {
        auto name = NameVisitors::assertSimpleUnique(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Class, node.location(), name));

        sym->setProperty("type", c.types.insert(Type::primary(sym)));
    }

    node.setProperty("sym", sym);

    if(node.body)
    {
        if(sym->property("defined"))
        {
            throw Error(node.location(), "class already defined - ", node.name->description());
        }

        sym->setProperty("defined", true);
    }

    if(node.body)
    {
        auto g = c.tree.open(sym);
        node.body->accept(*this);
    }
}

void Decorator::checkFunctionReturned(Context &c, FuncNode &node)
{
return;
    auto type = node.assertProperty("sym").to<Sym*>()->assertProperty("type").to<Type*>();
    if(type->returnType != c.types.nullType() && !node.body->assertProperty("sym").to<Sym*>()->property("returned"))
    {
        throw Error(node.location(), "function missing return - ", node.description());
    }
}
