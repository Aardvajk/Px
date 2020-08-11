#include "FuncGenerator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "generator/CommonGenerator.h"
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

void FuncGenerator::visit(VarNode &node)
{
    if(node.value)
    {
        auto size = ExprGenerator::generate(c, node.value.get(), os);

        os << "    push &\"" << node.assertProperty("sym").to<Sym*>()->fullname() << "\";\n";
        os << "    store " << size << ";\n";
        os << "    pop " << size << ";\n";
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

void FuncGenerator::visit(ReturnNode &node)
{
    if(node.expr)
    {
        auto size = ExprGenerator::generate(c, node.expr.get(), os);

        os << "    push &\"@ret\";\n";
        os << "    store " << size << ";\n";
        os << "    pop " << size << ";\n";
    }

    os << "    jump \"#function_epilogue\";\n";
}

void FuncGenerator::visit(IfNode &node)
{
    auto info = c.tree.current()->container()->assertProperty("info").to<FuncInfo*>();

    auto l0 = info->nextLabelQuoted();
    auto l1 = info->nextLabelQuoted();

    CommonGenerator::generateBooleanExpr(c, os, node.expr.get());
    os << "    jump ifz " << l0 << ";\n";

    node.body->accept(*this);
    os << "    jump " << l1 << ";\n";

    os << l0 << ":\n";

    if(node.elseBody)
    {
        node.elseBody->accept(*this);
    }

    os << l1 << ":\n";
}
