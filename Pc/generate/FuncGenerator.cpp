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

void FuncGenerator::visit(VarNode &node)
{
    if(node.value)
    {
        auto size = ExprGenerator::generate(c, os, node.value.get());

        os << "    push &\"" << node.assertedProperty("sym").to<Sym*>()->fullname() << "\";\n";
        os << "    store " << size << ";\n";
        os << "    pop " << size << ";\n";
    }
}

void FuncGenerator::visit(ExprNode &node)
{
    auto size = ExprGenerator::generate(c, os, node.expr.get());

    os << "    pop " << size << ";\n";
}

void FuncGenerator::visit(ReturnNode &node)
{
    if(node.expr)
    {
        auto sz = ExprGenerator::generate(c, os, node.expr.get());

        os << "    push &\"@ret\";\n";
        os << "    store " << sz << ";\n";
        os << "    pop " << sz << ";\n";
    }

    os << "    jump \"#end_function\";\n";
}
