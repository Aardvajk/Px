#include "TypeQuery.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/Type.h"

TypeQuery::TypeQuery(Context &c) : c(c), r(nullptr)
{
}

void TypeQuery::visit(IdNode &node)
{
    if(auto sym = node.property("sym"))
    {
        r = sym.to<Sym*>()->assertedProperty("type").to<Type*>();
    }
}

void TypeQuery::visit(TypeNode &node)
{
    r = node.assertedProperty("type").value<Type*>();
}

void TypeQuery::visit(VarNode &node)
{
    if(auto p = node.property("sym"))
    {
        r = p.to<Sym*>()->assertedProperty("type").to<Type*>();
    }
    else if(node.type)
    {
        node.type->accept(*this);
    }
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

Type *TypeQuery::assertCallable(Context &c, Node *node)
{
    auto t = assert(c, node);
    if(!t->returnType)
    {
        throw Error(node->location(), "type cannot be called - ", node->description());
    }

    return t;
}

