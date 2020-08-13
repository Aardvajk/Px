#include "VarDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include "decorator/ExprDecorator.h"

#include "visitors/NameVisitors.h"

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

        type = c.generics.updateType(node.type->property("type").to<Type*>());
        node.type->setProperty("type", type);
    }

    if(node.value)
    {
        ExprDecorator::decorate(c, node.value);

        auto valueType = TypeQuery::assert(c, node.value.get());
        if(type)
        {
            if(!Type::exact(type, valueType))
            {
                throw Error(node.value->location(), type->description(), " expected - ", node.value->description());
            }
        }
        else
        {
            type = valueType;
        }
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
