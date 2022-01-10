#include "TypeParser.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"

NodePtr TypeParser::build(Context &c, bool get)
{
    std::size_t ptr = 0;

    auto tok = c.scanner.next(get);
    while(tok.type() == Token::Type::RwPtr)
    {
        ++ptr;
        tok = c.scanner.next(true);
    }

    auto name = CommonParser::name(c, false);

    auto node = new TypeNode(name->location(), name);
    NodePtr n(node);

    node->ptr = ptr;

    return n;
}
