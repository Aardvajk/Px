#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/TypeQuery.h"

namespace
{

void checkLiteral(ExprGenerator::Flags flags, Node &node)
{
    if(flags & ExprGenerator::Flag::Addr)
    {
        throw Error(node.location(), "cannot take address of constant - ", node.description());
    }
}

}

ExprGenerator::ExprGenerator(Context &c, std::ostream &os, Flags flags) : c(c), os(os), flags(flags)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto sym = node.assertedProperty("sym").to<Sym*>();

    if(sym->type() == Sym::Type::Func)
    {
        os << "    push &\"" << sym->funcname() << "\";\n";
        r = sizeof(std::size_t);
    }
    else if(sym->property("member").value<bool>())
    {
        generate(c, os, node.parent.get(), Flag::Addr);

        auto size = sym->assertedProperty("type").to<Type*>()->assertedSize(node.location());

        os << "    incrs " << sym->assertedProperty("offset").to<std::size_t>() << ";\n";

        if(flags & Flag::Addr)
        {
            r = sizeof(std::size_t);
        }
        else
        {
            os << "    load " << size << ";\n";
            r = size;
        }
    }
    else
    {
        if(flags & Flag::Addr)
        {
            os << "    push &\"" << sym->fullname() << "\";\n";
            r = sizeof(std::size_t);
        }
        else
        {
            os << "    push \"" << sym->fullname() << "\";\n";
            r = sym->assertedProperty("type").to<Type*>()->assertedSize(node.location());
        }
    }
}

void ExprGenerator::visit(CallNode &node)
{
    auto type = TypeQuery::assertCallable(c, node.target.get());
    auto size = type->returnType->assertedSize(node.location());

    os << "    allocs " << size << ";\n";

    for(auto a: node.args)
    {
        generate(c, os, a.get());
    }

    generate(c, os, node.target.get());
    os << "    call;\n";

    r = size;
}

void ExprGenerator::visit(CharLiteralNode &node)
{
    checkLiteral(flags, node);

    os << "    push char(" << static_cast<int>(node.value) << ");\n";
    r = sizeof(char);
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    checkLiteral(flags, node);

    os << "    push int(" << node.value << ");\n";
    r = sizeof(int);
}

void ExprGenerator::visit(DerefNode &node)
{
    auto sz = TypeQuery::query(c, &node)->assertedSize(node.location());

    generate(c, os, node.expr.get());
    r = sizeof(std::size_t);

    if(!(flags & Flag::Addr))
    {
        os << "    load " << sz << ";\n";
        r = sz;
    }
}

void ExprGenerator::visit(AddrNode &node)
{
    if(flags & Flag::Addr)
    {
        throw Error(node.location(), "cannot take address of address - ", node.description());
    }

    generate(c, os, node.expr.get(), Flag::Addr);
    r = sizeof(std::size_t);
}

void ExprGenerator::visit(AssignNode &node)
{
    auto sz = generate(c, os, node.value.get());

    generate(c, os, node.target.get(), Flag::Addr);
    os << "    store " << sz << ";\n";

    r = sz;
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node *node, Flags flags)
{
    ExprGenerator g(c, os, flags);
    node->accept(g);

    if(!g.result())
    {
        throw Error(Error::Flag::Internal, node->location(), "expr generator failed - ", node->description());
    }

    return *g.result();
}
