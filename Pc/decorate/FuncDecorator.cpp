#include "FuncDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "types/TypeQuery.h"

#include "decorate/VarDecorator.h"
#include "decorate/ExprDecorator.h"

#include <pcx/str.h>

FuncDecorator::FuncDecorator(Context &c) : c(c)
{
}

void FuncDecorator::visit(BlockNode &node)
{
    for(auto n: node.nodes)
    {
        n->accept(*this);
    }
}

void FuncDecorator::visit(ScopeNode &node)
{
    auto info = c.tree.current()->container()->assertedProperty("info").to<FuncInfo*>();

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Scope, node.location(), pcx::str("#scope", info->scopes++)));
    node.setProperty("sym", sym);

    auto g = c.tree.open(sym);
    node.body->accept(*this);
}

void FuncDecorator::visit(VarNode &node)
{
    Visitor::visit<VarDecorator>(&node, c);
}

void FuncDecorator::visit(ExprNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);
}

void FuncDecorator::visit(ReturnNode &node)
{
    auto t = c.types.primitiveType(Primitive::Type::Null);

    if(node.expr)
    {
        node.expr = ExprDecorator::decorate(c, node.expr);
    }

    auto f = c.tree.current()->container()->assertedProperty("type").to<Type*>();

    if(f->returnType->ref)
    {
        auto n = new AddrNode(node.location());
        n->expr = node.expr;

        node.expr = n;
    }

    if(node.expr)
    {
        t = TypeQuery::assert(c, node.expr.get());
    }

    if(!Type::compare(f->returnType, t))
    {
        throw Error(node.location(), f->returnType->description(), " expected - ", node.description());
    }
}
