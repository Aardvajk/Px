#include "TypeParser.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"

NodePtr TypeParser::build(Context &c, bool get)
{
    bool ref = false;
    std::size_t ptr = 0;

    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::RwRef)
    {
        ref = true;
        tok = c.scanner.next(true);
    }

    while(tok.type() == Token::Type::RwPtr)
    {
        ++ptr;
        tok = c.scanner.next(true);
    }

    auto name = CommonParser::name(c, false);

    auto node = new TypeNode(name->location(), name);
    NodePtr n(node);

    node->ref = ref;
    node->ptr = ptr;

    return n;
}
