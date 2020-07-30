#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeQuery.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto sym = node.assertProperty("sym").to<Sym*>();

    if(sym->type() == Sym::Type::Func)
    {
        os << "    push &\"" << sym->funcname() << "\";\n";
        r = sizeof(std::size_t);
    }
}

void ExprGenerator::visit(CallNode &node)
{
    auto type = TypeQuery::assert(c, node.target.get());
    auto size = Type::assertSize(node.location(), type->returnType);

    os << "    allocs " << size << ";\n";
    generate(c, node.target.get(), os);
    os << "    call;\n";

    r = size;
}

void ExprGenerator::visit(CharLiteralNode &node)
{
    os << "    push char(" << int(node.value) << ");\n";
    os << "    service 2;\n";

    r = sizeof(char);
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    os << "    service 1;\n";

    r = sizeof(int);
}

std::size_t ExprGenerator::generate(Context &c, Node *node, std::ostream &os)
{
    ExprGenerator eg(c, os);
    node->accept(eg);

    if(!eg.result())
    {
        throw Error(node->location(), "expr generator failed - ", node->description());
    }

    return *eg.result();
}
