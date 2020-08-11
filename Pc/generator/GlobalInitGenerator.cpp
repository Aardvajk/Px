#include "GlobalInitGenerator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

GlobalInitGenerator::GlobalInitGenerator(Context &c, std::ostream &os) : c(c), os(os), ok(false)
{
}

void GlobalInitGenerator::visit(CharLiteralNode &node)
{
    os << "char(" << static_cast<int>(node.value) << ")";
    ok = true;
}

void GlobalInitGenerator::visit(IntLiteralNode &node)
{
    os << "int(" << node.value << ")";
    ok = true;
}

void GlobalInitGenerator::visit(BoolLiteralNode &node)
{
    os << "char(" << (node.value ? 1 : 0) << ")";
    ok = true;
}
