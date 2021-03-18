#include "AstPrinter.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include <pcx/scoped_counter.h>

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

void AstPrinter::visit(FuncNode &node)
{
    tab() << node.description() << "\n";

    if(node.body)
    {
        node.body->accept(*this);
    }
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
