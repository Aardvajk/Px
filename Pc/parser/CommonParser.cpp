#include "CommonParser.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/Parser.h"

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

NodePtr CommonParser::blockContents(Context &c, Location location, bool get)
{
    auto block = new BlockNode(location);
    NodePtr n(block);

    c.scanner.match(Token::Type::LeftBrace, get);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::RightBrace)
    {
        Parser::construct(c, block, false);
    }

    c.scanner.next(true);

    return n;
}

NodePtr CommonParser::scopeContents(Context &c, Location location, bool get)
{
    auto scope = new ScopeNode(location);
    NodePtr n(scope);

    auto tok = c.scanner.next(true);

    if(tok.type() == Token::Type::LeftBrace)
    {
        scope->body = blockContents(c, location, false);
    }
    else
    {
        auto block = new BlockNode(location);
        scope->body = block;

        Parser::construct(c, block, false);
    }

    return n;
}
