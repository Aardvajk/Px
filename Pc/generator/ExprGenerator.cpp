#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "generics/Generics.h"

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
        if(auto p = node.property("generics"))
        {
            os << "    push &\"" << Generics::funcname(sym, p.to<std::vector<Type*> >()) << "\";\n";
        }
        else
        {
            os << "    push &\"" << sym->funcname() << "\";\n";
        }

        r = sizeof(std::size_t);
    }
    else
    {
        os << "    push \"" << sym->fullname() << "\";\n";
        r = Type::assertSize(node.location(), sym->assertProperty("type").to<Type*>());
    }
}

void ExprGenerator::visit(CallNode &node)
{
    auto type = TypeQuery::assertCallable(c, node.target.get())->returnType;
    if(type->gref)
    {
        if(auto p = node.target->property("generics"))
        {
            type = p.to<std::vector<Type*> >()[type->gref->index];
        }
    }

    auto size = Type::assertSize(node.location(), type);

    os << "    allocs " << size << ";\n";

    for(auto &a: node.args)
    {
        generate(c, a.get(), os);
    }

    generate(c, node.target.get(), os);
    os << "    call;\n";

    r = size;
}

void ExprGenerator::visit(CharLiteralNode &node)
{
    os << "    push char(" << int(node.value) << ");\n";
    r = sizeof(char);
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
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
