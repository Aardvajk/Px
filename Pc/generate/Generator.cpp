#include "Generator.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "generate/LocalsGenerator.h"
#include "generate/FuncGenerator.h"

Generator::Generator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void Generator::visit(BlockNode &node)
{
    for(auto n: node.nodes)
    {
        n->accept(*this);
    }
}

void Generator::visit(NamespaceNode &node)
{
    node.body->accept(*this);
}

void Generator::visit(FuncNode &node)
{
    if(node.body)
    {
        auto sym = node.assertedProperty("sym").to<Sym*>();
        auto type = sym->assertedProperty("type").to<Type*>();

        os << "func \"" << sym->funcname() << "\":" << type->returnType->assertedSize(node.location()) << "\n";
        os << "{\n";

        for(auto a: node.args)
        {
            auto sym = a->assertedProperty("sym").to<Sym*>();
            auto type = sym->assertedProperty("type").to<Type*>();

            os << "    arg \"" << sym->fullname() << "\":" << type->assertedSize(a->location()) << ";\n";
        }

        Visitor::visit<LocalsGenerator>(node.body.get(), c, os);
        Visitor::visit<FuncGenerator>(node.body.get(), c, os);

        os << "\"#end_function\":\n";
        os << "}\n\n";
    }
}

void Generator::visit(TemplateFuncNode &node)
{
    for(auto i: node.instances)
    {
        i->accept(*this);
    }
}

void Generator::visit(VarNode &node)
{
    auto sym = node.assertedProperty("sym").to<Sym*>();

    if(!sym->property("member").value<bool>())
    {
        auto type = sym->assertedProperty("type").to<Type*>();

        os << "var \"" << sym->fullname() << "\":" << type->assertedSize(node.location()) << ";\n";
    }
}

void Generator::visit(ClassNode &node)
{
    if(node.body)
    {
        node.body->accept(*this);
    }
}
