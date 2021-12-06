#include "Decorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/SymFinder.h"

#include "types/TypeBuilder.h"

#include "visitors/NameVisitors.h"

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

Sym *searchFunc(Context &c, Node *node, Type *type)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func)
        {
            throw Error(node->location(), "func expected - ", s->fullname());
        }

        auto t = s->assertedProperty("type").to<Type*>();

        if(Type::compare(type->args, t->args))
        {
            if(!Type::compare(type->returnType, t->returnType))
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
    auto returnType = c.types.nullType();

    if(node.type)
    {
        returnType = TypeBuilder::build(c, node.type.get());
    }

    auto type = c.types.insert(Type::function(returnType, { }));

    auto sym = searchFunc(c, node.name.get(), type);
    if(!sym)
    {
        auto name = NameVisitors::assertSimpleUnique(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Func, node.location(), name));

        sym->setProperty("type", type);
    }

    node.setProperty("sym", sym);

    if(node.body)
    {
        if(sym->property("defined").value<bool>())
        {
            throw Error(node.location(), "function already defined - ", node.name->description());
        }

        sym->setProperty("defined", true);
    }
}
