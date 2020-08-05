#include "FuncDecorator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "decorator/VarDecorator.h"
#include "decorator/ExprDecorator.h"

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
    if(node.expr)
    {
        ExprDecorator::decorate(c, node.expr);
    }
}
