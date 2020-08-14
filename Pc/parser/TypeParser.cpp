#include "TypeParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/TypeNode.h"

#include "parser/CommonParser.h"

namespace
{

using TypeNodePtr = pcx::scoped_ptr<TypeNode>;

TypeNodePtr outer(Context &c, bool get);

void generics(Context &c, NodeList &container, bool get)
{
    auto t = outer(c, get);
    container.push_back(t.release());

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        generics(c, container, true);
    }
}

void arg(Context &c, NodeList &container, bool get)
{
    container.push_back(outer(c, get).release());

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        arg(c, container, true);
    }
}

TypeNodePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    TypeNodePtr n(new TypeNode(tok.location()));

    if(tok.type() == Token::Type::LeftParen)
    {
        n->function = true;

        c.scanner.next(true);
        if(c.scanner.token().type() != Token::Type::RightParen)
        {
            arg(c, n->args, false);
        }

        c.scanner.match(Token::Type::RightParen, false);

        tok = c.scanner.next(true);
        if(tok.type() == Token::Type::Colon)
        {
            n->returnType = NodePtr(outer(c, true).release());
        }
    }
    else
    {
        n->name = CommonParser::name(c, false);

        if(c.scanner.token().type() == Token::Type::Lt)
        {
            generics(c, n->generics, true);
            c.scanner.consume(Token::Type::Gt, false);
        }
    }

    return n;
}

TypeNodePtr outer(Context &c, bool get)
{
    return primary(c, get);
}

}

NodePtr TypeParser::build(Context &c, bool get)
{
    return NodePtr(outer(c, get).release());
}
