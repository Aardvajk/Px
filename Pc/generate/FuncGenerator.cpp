#include "FuncGenerator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "generate/ExprGenerator.h"

FuncGenerator::FuncGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void FuncGenerator::visit(BlockNode &node)
{
    for(auto n: node.nodes)
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
    auto size = ExprGenerator::generate(c, os, node.expr.get());

    os << "    pop " << size << ";\n";
}