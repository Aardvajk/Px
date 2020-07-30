#include "ExprParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

namespace
{

NodePtr id(Context &c, NodePtr parent, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    auto node = new IdNode(tok.location(), parent, tok.text());
    NodePtr n(node);

    c.scanner.next(true);
    return n;
}

NodePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::Id: return id(c, { }, false);

        default: throw Error(tok.location(), "primary expected - ", tok.text());
    }
}

NodePtr call(Context &c, NodePtr target, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftParen, get);

    auto node = new CallNode(tok.location(), target);
    NodePtr n(node);

    c.scanner.consume(Token::Type::RightParen, true);

    return n;
}

NodePtr entity(Context &c, bool get)
{
    auto n = primary(c, get);

    while(true)
    {
        switch(c.scanner.token().type())
        {
            case Token::Type::Dot: n = id(c, n, true); break;
            case Token::Type::LeftParen: n = call(c, n, false); break;

            default: return n;
        }
    }
}

}

NodePtr ExprParser::build(Context &c, bool get)
{
    return entity(c, get);
}
