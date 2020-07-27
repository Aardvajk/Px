#include "AstPrinter.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/FuncNode.h"
#include "nodes/TypeNode.h"

#include <pcx/scoped_counter.h>

AstPrinter::AstPrinter(Context &c, std::ostream &os) : c(c), os(os), tc(0)
{
}

void AstPrinter::visit(BlockNode &node)
{
    tab() << "block\n";
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
    tab() << "func " << node.name->description() << "():" << node.type->description() << "\n";
}

void AstPrinter::visit(TypeNode &node)
{
    tab() << "type " << node.name->description() << "\n";
}

std::ostream &AstPrinter::tab() const
{
    return os << std::string(tc * 4, ' ');
}
