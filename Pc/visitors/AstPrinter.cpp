#include "AstPrinter.h"

#include "nodes/Nodes.h"

#include "syms/Sym.h"

#include <pcx/scoped_counter.h>

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
    tab() << "func " << node.name->description() << info(&node) << "\n";

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

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
