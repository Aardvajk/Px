#include "Generator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "generator/LocalsGenerator.h"
#include "generator/FuncGenerator.h"
#include "generator/GlobalInitGenerator.h"

Generator::Generator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void Generator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Generator::visit(NamespaceNode &node)
{
    auto g = c.tree.open(node.property("sym").to<Sym*>());
    node.body->accept(*this);
}

void Generator::visit(FuncNode &node)
{
    if(node.body && node.genericTags.empty())
    {
        auto sym = node.property("sym").to<Sym*>();
        auto type = sym->property("type").to<Type*>();

        os << "func";

        if(node.property("autogen"))
        {
            os << "[autogen]";
        }

        os << " \"" << sym->funcname() << "\":" << Type::assertSize(node.location(), type->returnType) << "\n";
        os << "{\n";

        for(auto &a: node.args)
        {
            auto sym = a->assertProperty("sym").to<Sym*>();
            os << "    arg \"" << sym->fullname() << "\":" << Type::assertSize(a->location(), sym->assertProperty("type").to<Type*>()) << ";\n";
        }

        auto g = c.tree.open(sym);

        Visitor::visit<LocalsGenerator>(node.body.get(), c, os);
        Visitor::visit<FuncGenerator>(node.body.get(), c, os);

        os << "\"#function_epilogue\":\n";
        os << "}\n";
    }
}

void Generator::visit(VarNode &node)
{
    auto sym = node.assertProperty("sym").to<Sym*>();
    auto type = sym->assertProperty("type").to<Type*>();

    if(!sym->property("member"))
    {
        os << "var \"" << sym->fullname() << "\":" << Type::assertSize(node.location(), type);

        if(node.value)
        {
            os << " = ";

            if(!Visitor::query<GlobalInitGenerator, bool>(node.value.get(), c, os))
            {
                throw Error(node.value->location(), "unable to generate global init - ", node.value->description());
            }
        }

        os << ";\n";
    }
}

void Generator::visit(ClassNode &node)
{
    if(node.body && node.genericTags.empty())
    {
        auto g = c.tree.open(node.property("sym").to<Sym*>());
        node.body->accept(*this);
    }
}
