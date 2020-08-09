#include "FuncDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "decorator/VarDecorator.h"
#include "decorator/ExprDecorator.h"

#include "types/TypeQuery.h"

#include <pcx/str.h>

FuncDecorator::FuncDecorator(Context &c) : c(c)
{
}

void FuncDecorator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void FuncDecorator::visit(VarNode &node)
{
    Visitor::visit<VarDecorator>(&node, c);
}

void FuncDecorator::visit(ScopeNode &node)
{
    auto info = c.tree.current()->container()->assertProperty("info").to<FuncInfo*>();

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Scope, node.location(), pcx::str("#scope", info->scopes++)));
    node.setProperty("sym", sym);

    auto g = c.tree.open(sym);
    node.body->accept(*this);
}

void FuncDecorator::visit(ExprNode &node)
{
    ExprDecorator::decorate(c, node.expr);
}

void FuncDecorator::visit(ReturnNode &node)
{
    auto expected = c.tree.current()->container()->assertProperty("type").to<Type*>()->returnType;

    Type *actual = nullptr;
    if(node.expr)
    {
        ExprDecorator::decorate(c, node.expr);
        actual = TypeQuery::assert(c, node.expr.get());
    }
    else
    {
        actual = c.types.nullType();
    }

    if(!Type::exact(expected, actual))
    {
        throw Error(node.location(), expected->description(), " return expected - ", actual->description());
    }

    c.tree.current()->setProperty("returned", true);
}
