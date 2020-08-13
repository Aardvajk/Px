#include "AstPrinter.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include <pcx/scoped_counter.h>
#include <pcx/join_str.h>

#include <sstream>

namespace
{

std::string info(Node *node)
{
    std::ostringstream os;

    if(auto p = node->property("sym"))
    {
        auto s = p.to<Sym*>();
        os << " -> " << Sym::toString(s->type()) << " " << (s->type() == Sym::Type::Func ? s->funcname() : s->fullname());
    }

    return os.str();
}

}

AstPrinter::AstPrinter(Context &c, std::ostream &os) : c(c), os(os), tc(0)
{
}

void AstPrinter::visit(BlockNode &node)
{
    tab() << "{\n";

    if(true)
    {
        auto g = pcx::scoped_counter(tc);
        for(auto &n: node.nodes)
        {
            n->accept(*this);
        }
    }

    tab() << "}\n";
}

void AstPrinter::visit(IdNode &node)
{
    tab() << "id " << node.name;

    if(!node.generics.empty())
    {
        os << "<" << pcx::join_str(node.generics, ", ") << ">";
    }

    os << info(&node) << "\n";

    if(node.parent)
    {
        auto g = pcx::scoped_counter(tc);
        node.parent->accept(*this);
    }
}

void AstPrinter::visit(GenericTagNode &node)
{
    tab() << "generic tag " << node.name << "\n";
}

void AstPrinter::visit(NamespaceNode &node)
{
    tab() << "namespace " << node.name->description() << info(&node) << "\n";

    node.body->accept(*this);
}

void AstPrinter::visit(FuncNode &node)
{
    tab() << node.description() << info(&node) << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
}

void AstPrinter::visit(TypeNode &node)
{
    tab() << "type " << node.name->description() << "\n";
}

void AstPrinter::visit(VarNode &node)
{
    tab() << "var " << node.description() << "\n";

    if(node.value)
    {
        auto g = pcx::scoped_counter(tc);
        node.value->accept(*this);
    }
}

void AstPrinter::visit(ClassNode &node)
{
    tab() << node.description() << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
}

void AstPrinter::visit(ScopeNode &node)
{
    node.body->accept(*this);
}

void AstPrinter::visit(ExprNode &node)
{
    tab() << "expr\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}

void AstPrinter::visit(CallNode &node)
{
    tab() << "call\n";

    auto g = pcx::scoped_counter(tc);
    node.target->accept(*this);

    if(!node.args.empty())
    {
        auto g = pcx::scoped_counter(tc);
        for(auto &a: node.args)
        {
            a->accept(*this);
        }
    }
}

void AstPrinter::visit(CharLiteralNode &node)
{
    tab() << "char literal " << int(node.value) << "\n";
}

void AstPrinter::visit(IntLiteralNode &node)
{
    tab() << "int literal " << node.value << "\n";
}

void AstPrinter::visit(BoolLiteralNode &node)
{
    tab() << "bool literal " << (node.value ? "true" : "false") << "\n";
}

void AstPrinter::visit(ReturnNode &node)
{
    tab() << "return\n";

    if(node.expr)
    {
        auto g = pcx::scoped_counter(tc);
        node.expr->accept(*this);
    }
}

void AstPrinter::visit(IfNode &node)
{
    tab() << "if\n";

    auto g = pcx::scoped_counter(tc);

    node.expr->accept(*this);
    node.body->accept(*this);

    if(node.elseBody)
    {
        tab() << "else\n";
        node.elseBody->accept(*this);
    }
}

void AstPrinter::visit(CommaNode &node)
{
    tab() << "comma\n";

    auto g = pcx::scoped_counter(tc);
    node.left->accept(*this);
    node.right->accept(*this);
}
