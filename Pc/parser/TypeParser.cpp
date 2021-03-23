#include "TypeParser.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"

NodePtr TypeParser::build(Context &c, bool get)
{
    auto name = CommonParser::name(c, get);

    auto node = new TypeNode(name->location(), name);
    NodePtr n(node);

    return n;
}
