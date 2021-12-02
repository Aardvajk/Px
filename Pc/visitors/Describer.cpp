#include "Describer.h"

Describer::Describer()
{
}

void Describer::visit(BlockNode &node)
{
    r += "(block)";
}
