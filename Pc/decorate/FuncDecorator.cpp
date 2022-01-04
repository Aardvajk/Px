#include "FuncDecorator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

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
    Visitor::visit<ExprDecorator>(node.expr.get(), c);
}

void FuncDecorator::visit(ReturnNode &node)
{
    Visitor::visit<ExprDecorator>(node.expr.get(), c);
}
