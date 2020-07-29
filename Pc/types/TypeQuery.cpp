#include "TypeQuery.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

TypeQuery::TypeQuery(Context &c) : c(c), r(nullptr)
{
}

void TypeQuery::visit(TypeNode &node)
{
    r = node.property("type").value<Type*>();
}

void TypeQuery::visit(VarNode &node)
{
    node.type->accept(*this);
}

Type *TypeQuery::query(Context &c, Node *node)
{
    TypeQuery v(c);
    node->accept(v);

    return v.result();
}

Type *TypeQuery::assert(Context &c, Node *node)
{
    auto t = query(c, node);
    if(!t)
    {
        throw Error(node->location(), "type query failed - ", node->description());
    }

    return t;
}
