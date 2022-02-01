#include "VarDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "visitors/NameVisitors.h"

#include "decorate/ExprDecorator.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

VarDecorator::VarDecorator(Context &c) : c(c)
{
}

void VarDecorator::visit(VarNode &node)
{
    auto name = NameVisitors::assertSimpleUnique(c, node.name.get());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Var, node.location(), name));
    node.setProperty("sym", sym);

    Type *type = nullptr;

    if(node.type)
    {
        if(node.ref)
        {
            throw Error(node.location(), "reference cannot have explicit type - ", node.description());
        }

        if(!node.type->property("type"))
        {
            node.type->setProperty("type", TypeBuilder::build(c, node.type.get()));
        }

        type = node.type->assertedProperty("type").to<Type*>();
        node.type->setProperty("type", type);
    }

    if(node.value)
    {
        node.value = ExprDecorator::decorate(c, node.value);

        auto t = TypeQuery::assert(c, node.value.get());

        if(type)
        {
            if(!Type::compare(type, t))
            {
                throw Error(node.location(), "initialiser of wrong type - ", node.description());
            }
        }
        else
        {
            auto ft = t->removeRef();

            if(node.ref)
            {
                ft.ref = true;
            }

            type = c.types.insert(ft);
        }
    }
    else if(node.ref)
    {
        throw Error(node.location(), "reference must be initialised - ", node.description());
    }

    if(!type)
    {
        throw Error(node.location(), "no type provided - ", node.description());
    }

    if(type->ref)
    {
        type = c.types.insert(type->addPointer());

        if(!node.property("arg").value<bool>())
        {
            if(!node.value)
            {
                throw Error(node.location(), "ref must be initialised - ", node.description());
            }

            auto a = new AddrNode(node.location());
            a->expr = node.value;

            node.value = a;
        }
    }

    sym->setProperty("type", type);

    if(c.tree.current()->container()->type() == Sym::Type::Class)
    {
        sym->setProperty("member", true);
    }
}
