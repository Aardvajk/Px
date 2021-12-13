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
    tab() << "id " << node.name << info(&node) << "\n";

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

void AstPrinter::visit(ScopeNode &node)
{
    tab() << "(scope)" << info(&node) << "\n";
    node.body->accept(*this);
}

void AstPrinter::visit(TypeNode &node)
{
    tab() << "type " << node.name->description() << "\n";
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
    tab() << "class " << node.name->description() << info(&node) << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
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
