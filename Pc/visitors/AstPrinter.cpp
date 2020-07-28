#include "AstPrinter.h"

#include "nodes/Nodes.h"

#include <pcx/scoped_counter.h>
#include <pcx/join_str.h>

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
    tab() << "id " << node.name << "\n";

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
    tab() << "namespace " << node.name->description() << "\n";

    node.body->accept(*this);
}

void AstPrinter::visit(FuncNode &node)
{
    tab() << node.description() << "\n";
}

void AstPrinter::visit(TypeNode &node)
{
    tab() << "type " << node.name->description() << "\n";
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
