#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "generics/Generics.h"

#include "types/Type.h"
#include "types/TypeQuery.h"

#include <pcx/indexed_range.h>

namespace
{

Type *updateType(Type *type, Node *target)
{
    if(type->gref)
    {
        if(auto p = target->property("generics"))
        {
            type = p.to<std::vector<Type*> >()[type->gref->index];
        }
    }

    return type;
}

}

ExprGenerator::ExprGenerator(Context &c, std::ostream &os, Flags flags) : c(c), os(os), flags(flags)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto sym = node.assertProperty("sym").to<Sym*>();

    if(sym->type() == Sym::Type::Func)
    {
        auto name = sym->funcname();
        if(auto p = node.property("generics"))
        {
            name = Generics::funcname(sym, p.to<std::vector<Type*> >());
        }

        os << "    push &\"" << name << "\";\n";
        r = sizeof(std::size_t);
    }
    else if(sym->property("member"))
    {
        generate(c, node.parent.get(), os, Flag::Addr);

        auto size = Type::assertSize(node.location(), sym->assertProperty("type").to<Type*>());

        os << "    incrs " << sym->assertProperty("offset").to<std::size_t>() << ";\n";

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
            r = Type::assertSize(node.location(), sym->assertProperty("type").to<Type*>());
        }
    }
}

void ExprGenerator::visit(CallNode &node)
{
    auto type = TypeQuery::assertCallable(c, node.target.get());
    auto returnType = updateType(type->returnType, node.target.get());

    auto size = Type::assertSize(node.location(), returnType);

    os << "    allocs " << size << ";\n";

    for(auto a: pcx::indexed_range(node.args))
    {
        auto argType = updateType(type->args[a.index], node.target.get());

        if(!Type::exact(TypeQuery::assert(c, a.value.get()), argType))
        {
            throw Error(a.value->location(), argType->description(), " expected - ", a.value->description());
        }

        generate(c, a.value.get(), os);
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

void ExprGenerator::visit(BoolLiteralNode &node)
{
    os << "    push char(" << static_cast<int>(node.value) << ");\n";
    r = sizeof(char);
}

void ExprGenerator::visit(CommaNode &node)
{
    auto size = generate(c, node.left.get(), os);
    os << "    pop " << size << ";\n";

    r = generate(c, node.right.get(), os);
}

std::size_t ExprGenerator::generate(Context &c, Node *node, std::ostream &os, Flags flags)
{
    ExprGenerator eg(c, os, flags);
    node->accept(eg);

    if(!eg.result())
    {
        throw Error(node->location(), "expr generator failed - ", node->description());
    }

    return *eg.result();
}
