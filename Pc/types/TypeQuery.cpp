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
        if(auto t = sym.to<Sym*>()->property("type"))
        {
            r = t.to<Type*>();
        }
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

void TypeQuery::visit(DerefNode &node)
{
    r = c.types.insert(Visitor::query<TypeQuery, Type*>(node.expr.get(), c)->removePointer());
}

void TypeQuery::visit(AddrNode &node)
{
    r = c.types.insert(Visitor::query<TypeQuery, Type*>(node.expr.get(), c)->addPointer());
}

void TypeQuery::visit(AssignNode &node)
{
    node.target->accept(*this);
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

    Error::assert(t, node->location(), "type query failed - ", node->description());

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

