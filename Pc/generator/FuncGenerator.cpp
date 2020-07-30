#include "FuncGenerator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "generator/ExprGenerator.h"

FuncGenerator::FuncGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void FuncGenerator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void FuncGenerator::visit(ScopeNode &node)
{
    node.body->accept(*this);
}

void FuncGenerator::visit(ExprNode &node)
{
    auto size = ExprGenerator::generate(c, node.expr.get(), os);

    os << "    pop " << size << ";\n";
}
