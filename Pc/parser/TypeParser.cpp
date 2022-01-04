#include "TypeParser.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"

NodePtr TypeParser::build(Context &c, bool get)
{
    auto name = CommonParser::name(c, get);

    return new TypeNode(name->location(), name);
}

NodePtr TypeParser::buildArg(Context &c, bool get)
{
    auto name = CommonParser::argName(c, get);

    return new TypeNode(name->location(), name);
}
