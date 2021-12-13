#include "VarDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "visitors/NameVisitors.h"

#include "types/TypeBuilder.h"

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
        if(!node.type->property("type"))
        {
            node.type->setProperty("type", TypeBuilder::build(c, node.type.get()));
        }

        type = node.type->assertedProperty("type").to<Type*>();
        node.type->setProperty("type", type);
    }

    if(!type)
    {
        throw Error(node.location(), "no type provided - ", node.description());
    }

    sym->setProperty("type", type);

    if(c.tree.current()->container()->type() == Sym::Type::Class)
    {
        sym->setProperty("member", true);
    }
}