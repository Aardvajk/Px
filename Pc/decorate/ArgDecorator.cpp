#include "ArgDecorator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "types/TypeBuilder.h"

ArgDecorator::ArgDecorator(Context &c) : c(c)
{
}

void ArgDecorator::visit(VarNode &node)
{
    node.type->setProperty("type", TypeBuilder::build(c, node.type.get()));
}
