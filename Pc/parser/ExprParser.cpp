#include "ExprParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

namespace
{

NodePtr expression(Context &c, bool get);

NodePtr id(Context &c, NodePtr parent, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    auto node = new IdNode(tok.location(), parent, tok.text());
    NodePtr n(node);

    c.scanner.next(true);
    return n;
}

void args(Context &c, NodeList &container, bool get)
{
    container.push_back(expression(c, get));

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        args(c, container, true);
    }
}

NodePtr call(Context &c, NodePtr target, bool get)
{
    auto node = new CallNode(c.scanner.token().location());
    NodePtr n(node);

    node->target = target;

    auto tok = c.scanner.next(get);
    if(tok.type() != Token::Type::RightParen)
    {
        args(c, node->args, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);
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

NodePtr entity(Context &c, bool get)
{
    auto n = primary(c, get);

    while(true)
    {
        switch(c.scanner.token().type())
        {
            case Token::Type::Dot: n = id(c, n, true); break;
            case Token::Type::LeftParen: n = call(c, n, true); break;

            default: return n;
        }
    }
}

NodePtr expression(Context &c, bool get)
{
    return entity(c, get);
}

}

NodePtr ExprParser::build(Context &c, bool get)
{
    return expression(c, get);
}
