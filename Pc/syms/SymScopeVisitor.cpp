#include "SymScopeVisitor.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeQuery.h"

SymScopeVisitor::SymScopeVisitor(Context &c, Sym *curr) : c(c), curr(curr)
{
}

void SymScopeVisitor::visit(CallNode &node)
{
    curr = TypeQuery::assertCallable(c, node.target.get())->returnType->sym;
}

void SymScopeVisitor::visit(DerefNode &node)
{
    node.expr->accept(*this);
}

void SymScopeVisitor::visit(ThisNode &node)
{
    curr = curr->container()->parent();
}
