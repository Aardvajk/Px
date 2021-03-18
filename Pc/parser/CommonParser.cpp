#include "CommonParser.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

namespace
{

NodePtr nameImp(Context &c, NodePtr parent, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    c.scanner.next(true);

    auto id = new IdNode(tok.location(), parent, tok.text());
    NodePtr n(id);

    while(c.scanner.token().type() == Token::Type::Dot)
    {
        n = nameImp(c, n, true);
    }

    return n;
}

}

NodePtr CommonParser::name(Context &c, bool get)
{
    return nameImp(c, { }, get);
}
