#include "TypeQuery.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "generics/Generics.h"

TypeQuery::TypeQuery(Context &c) : c(c), r(nullptr)
{
}

void TypeQuery::visit(IdNode &node)
{
    if(auto sym = node.property("sym"))
    {
        r = sym.to<Sym*>()->assertProperty("type").to<Type*>();

        if(auto p = node.property("generics"))
        {
            if(r->function() && r->returnType->gref)
            {
                auto t = *r;
                t.returnType = p.to<std::vector<Type*> >()[t.returnType->gref->index];

                r = c.types.insert(t);
            }
        }
    }
}

void TypeQuery::visit(TypeNode &node)
{
    r = node.property("type").value<Type*>();
}

void TypeQuery::visit(VarNode &node)
{
    if(auto p = node.property("sym"))
    {
        r = p.to<Sym*>()->assertProperty("type").to<Type*>();
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

void TypeQuery::visit(BoolLiteralNode &node)
{
    r = c.types.boolType();
}

void TypeQuery::visit(CommaNode &node)
{
    node.right->accept(*this);
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
    if(!t->function())
    {
        throw Error(node->location(), "callable expected - ", node->description());
    }

    return t;
}
