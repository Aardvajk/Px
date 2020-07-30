#include "TypeQuery.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

TypeQuery::TypeQuery(Context &c) : c(c), r(nullptr)
{
}

void TypeQuery::visit(IdNode &node)
{
    if(auto sym = node.property("sym"))
    {
        r = sym.to<Sym*>()->assertProperty("type").to<Type*>();
    }
}

void TypeQuery::visit(TypeNode &node)
{
    r = node.property("type").value<Type*>();
}

void TypeQuery::visit(VarNode &node)
{
    node.type->accept(*this);
}

void TypeQuery::visit(CallNode &node)
{
    r = assert(c, node.target.get())->returnType;
}

void TypeQuery::visit(CharLiteralNode &node)
{
    r = c.types.charType();
}

void TypeQuery::visit(IntLiteralNode &node)
{
    r = c.types.intType();
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
