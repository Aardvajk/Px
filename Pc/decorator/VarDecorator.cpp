#include "VarDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "types/TypeBuilder.h"
#include "types/TypeQuery.h"

#include "visitors/NameVisitors.h"

VarDecorator::VarDecorator(Context &c) : c(c)
{
}

void VarDecorator::visit(VarNode &node)
{
    auto name = NameVisitors::assertSimpleUnique(c, node.name.get());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Var, node.location(), name));
    node.setProperty("sym", sym);

    if(!node.type->property("type"))
    {
        node.type->setProperty("type", TypeBuilder::build(c, node.type.get()));
    }

    sym->setProperty("type", TypeQuery::assert(c, node.type.get()));
}
