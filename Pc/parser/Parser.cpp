#include "Parser.h"

#include "nodes/BlockNode.h"

NodePtr Parser::build(Context &c)
{
    auto block = new BlockNode({ });
    NodePtr n(block);

    return n;
}
