#include "ExprParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/TypeParser.h"

#include <pcx/lexical_cast.h>

namespace
{

NodePtr entity(Context &c, bool get);
NodePtr expression(Context &c, bool get);

using Func = NodePtr(*)(Context&,bool);

NodePtr paren(Context &c, bool get)
{
    auto n = expression(c, get);

    c.scanner.consume(Token::Type::RightParen, false);

    return n;
}

void templateParam(Context &c, NodeList &container, bool get)
{
    container.push_back(TypeParser::build(c, get));

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        templateParam(c, container, true);
    }
}

NodePtr id(Context &c, NodePtr parent, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    auto node = new IdNode(tok.location(), parent, tok.text());
    NodePtr n(node);

    c.scanner.next(true);
    if(c.scanner.token().type() == Token::Type::Lt)
    {
        c.scanner.next(true);
        if(c.scanner.token().type() != Token::Type::Gt)
        {
            templateParam(c, node->params, false);
        }

        c.scanner.consume(Token::Type::Gt, false);
    }

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

template<typename T> NodePtr exprNode(Context &c, Func fn, bool get)
{
    auto node = new T(c.scanner.token().location());
    NodePtr n(node);

    node->expr = fn(c, get);

    return n;
}

NodePtr primary(Context &c, bool get)
{
    NodePtr n;

    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::LeftParen: return paren(c, true);

        case Token::Type::Id: return id(c, { }, false);

        case Token::Type::CharLiteral: n = new CharLiteralNode(tok.location(), tok.text()[0]); c.scanner.next(true); return n;
        case Token::Type::IntLiteral: n = new IntLiteralNode(tok.location(), pcx::lexical_cast<int>(tok.text())); c.scanner.next(true); return n;

        case Token::Type::Star: return exprNode<DerefNode>(c, entity, true);
        case Token::Type::Amp: return exprNode<AddrNode>(c, entity, true);

        case Token::Type::RwThis: n = new ThisNode(tok.location()); c.scanner.next(true); return n;

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

NodePtr assign(Context &c, bool get)
{
    auto n = entity(c, get);

    while(c.scanner.token().type() == Token::Type::Assign)
    {
        auto an = new AssignNode(c.scanner.token().location(), n);
        n = an;

        an->value = expression(c, true);
    }

    return n;
}

NodePtr expression(Context &c, bool get)
{
    return assign(c, get);
}

}

NodePtr ExprParser::build(Context &c, bool get)
{
    return expression(c, get);
}
