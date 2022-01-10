#include "AstPrinter.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include "types/Type.h"

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
        os << " -> " << Sym::toString(s->type()) << " " << s->fullname();

        if(auto p = s->property("type"))
        {
            os << ":" << p.to<Type*>()->description();
        }
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

    if(!node.params.empty())
    {
        os << "<" << pcx::join_str(node.params, ",") << ">";
    }

    os << info(&node) << "\n";

    if(node.parent)
    {
        auto g = pcx::scoped_counter(tc);
        node.parent->accept(*this);
    }
}

void AstPrinter::visit(NamespaceNode &node)
{
    tab() << "namespace " << node.name->description() << info(&node) << "\n";

    node.body->accept(*this);
}

void AstPrinter::visit(FuncNode &node)
{
    tab() << "func " << node.name->description() << "(" << pcx::join_str(node.args, ",") << ")";

    if(node.type)
    {
        os << ":" << node.type->description();
    }

    os << info(&node) << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
}

void AstPrinter::visit(TemplateFuncNode &node)
{
    tab() << "template func " << node.name->description() << "<" << pcx::join_str(node.params, ",") << ">(" << pcx::join_str(node.args, ",") << ")";

    if(node.type)
    {
        os << ":" << node.type->description();
    }

    os << info(&node) << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }

    if(!node.instances.empty())
    {
        tab() << "instances\n";
        tab() << "{\n";

        for(auto i: node.instances)
        {
            auto g = pcx::scoped_counter(tc);
            i->accept(*this);
        }

        tab() << "}\n";
    }
}

void AstPrinter::visit(ScopeNode &node)
{
    tab() << "(scope)" << info(&node) << "\n";
    node.body->accept(*this);
}

void AstPrinter::visit(TypeNode &node)
{
    tab() << "type " << node.description() << "\n";
}

void AstPrinter::visit(VarNode &node)
{
    tab() << "var " << node.name->description();

    if(node.type)
    {
        os << ":" << node.type->description();
    }

    os << "\n";
}

void AstPrinter::visit(ClassNode &node)
{
    tab() << "class " << node.description() << info(&node) << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
}

void AstPrinter::visit(TemplateClassNode &node)
{
    tab() << "template class " << node.description() << info(&node) << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }

    if(!node.instances.empty())
    {
        tab() << "instances\n";
        tab() << "{\n";

        for(auto i: node.instances)
        {
            auto g = pcx::scoped_counter(tc);
            i->accept(*this);
        }

        tab() << "}\n";
    }
}

void AstPrinter::visit(ExprNode &node)
{
    tab() << "expr\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(CallNode &node)
{
    tab() << "call\n";

    auto g1 = pcx::scoped_counter(tc);
    node.target->accept(*this);

    auto g2 = pcx::scoped_counter(tc);
    for(auto a: node.args)
    {
        a->accept(*this);
    }
}

void AstPrinter::visit(CharLiteralNode &node)
{
    tab() << "char literal " << node.description() << "\n";
}

void AstPrinter::visit(IntLiteralNode &node)
{
    tab() << "int literal " << node.description() << "\n";
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

void AstPrinter::visit(DerefNode &node)
{
    tab() << "deref\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(AddrNode &node)
{
    tab() << "addrof\n";

    auto g = pcx::scoped_counter(tc);
    node.expr->accept(*this);
}

void AstPrinter::visit(AssignNode &node)
{
    tab() << "assign\n";

    auto g = pcx::scoped_counter(tc);

    node.target->accept(*this);
    node.value->accept(*this);
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
