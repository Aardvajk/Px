#include "TypeParser.h"

#include "nodes/TypeNode.h"

#include "parser/CommonParser.h"

NodePtr TypeParser::build(Context &c, bool get)
{
    auto name = CommonParser::name(c, get);

    auto type = new TypeNode(name->location());
    NodePtr n(type);

    type->name = name;

    return n;
}
