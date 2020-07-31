#include "LocalsGenerator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

LocalsGenerator::LocalsGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void LocalsGenerator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void LocalsGenerator::visit(VarNode &node)
{
    auto sym = node.assertProperty("sym").to<Sym*>();
    os << "    var \"" << sym->fullname() << "\":" << Type::assertSize(node.location(), sym->assertProperty("type").to<Type*>()) << ";\n";
}

void LocalsGenerator::visit(ScopeNode &node)
{
    auto sym = node.assertProperty("sym").to<Sym*>();

    auto g = c.tree.open(sym);
    node.body->accept(*this);
}
