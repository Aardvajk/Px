#include "ExprParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/TypeParser.h"

#include <pcx/lexical_cast.h>

namespace
{

NodePtr expressionList(Context &c, bool get);
NodePtr expression(Context &c, bool get);

void generics(Context &c, NodeList &container, bool get)
{
    auto type = TypeParser::build(c, get);
    container.push_back(type);

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        generics(c, container, true);
    }
}

NodePtr id(Context &c, NodePtr parent, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    auto node = new IdNode(tok.location(), parent, tok.text());
    NodePtr n(node);

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Lt)
    {
        generics(c, node->generics, true);
        c.scanner.consume(Token::Type::Gt, false);
    }

    return n;
}

NodePtr primary(Context &c, bool get)
{
    NodePtr n;

    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::Id: return id(c, { }, false);

        case Token::Type::CharLiteral: n = new CharLiteralNode(tok.location(), tok.text()[0]); c.scanner.next(true); return n;
        case Token::Type::IntLiteral: n = new IntLiteralNode(tok.location(), pcx::lexical_cast<int>(tok.text())); c.scanner.next(true); return n;

        case Token::Type::RwTrue: n = new BoolLiteralNode(tok.location(), true); c.scanner.next(true); return n;
        case Token::Type::RwFalse: n = new BoolLiteralNode(tok.location(), false); c.scanner.next(true); return n;

        default: throw Error(tok.location(), "primary expected - ", tok.text());
    }
}

void args(Context &c, NodeList &container, bool get)
{
    auto arg = expression(c, get);
    container.push_back(arg);

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        args(c, container, true);
    }
}

NodePtr call(Context &c, NodePtr target, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftParen, get);

    auto node = new CallNode(tok.location(), target);
    NodePtr n(node);

    tok = c.scanner.next(true);
    if(tok.type() != Token::Type::RightParen)
    {
        args(c, node->args, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);

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

NodePtr expressionList(Context &c, bool get)
{
    auto n = expression(c, get);

    while(c.scanner.token().type() == Token::Type::Comma)
    {
        n = new CommaNode(c.scanner.token().location(), n, expressionList(c, true));
    }

    return n;
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

NodePtr ExprParser::buildList(Context &c, bool get)
{
    return expressionList(c, get);
}
