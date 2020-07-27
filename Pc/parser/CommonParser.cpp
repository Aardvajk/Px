#include "CommonParser.h"

#include "application/Context.h"

#include "nodes/IdNode.h"

namespace
{

NodePtr nameImp(Context &c, NodePtr parent, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    c.scanner.next(true);

    auto n = new IdNode(tok.location(), parent, tok.text());
    NodePtr nn(n);

    while(c.scanner.token().type() == Token::Type::Dot)
    {
        nn = nameImp(c, nn, true);
    }

    return nn;
}

}

NodePtr CommonParser::name(Context &c, bool get)
{
    return nameImp(c, { }, get);
}
