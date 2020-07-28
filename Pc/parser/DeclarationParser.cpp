#include "DeclarationParser.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"
#include "parser/TypeParser.h"

namespace
{

void buildNamespace(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    auto n = new NamespaceNode(name->location(), name);
    block->push_back(n);

    n->body = CommonParser::blockContents(c, c.scanner.token().location(), false);
}

void buildGenericTags(Context &c, NodeList &container, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);
    container.push_back(new GenericTagNode(tok.location(), tok.text()));

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Comma)
    {
        buildGenericTags(c, container, true);
    }
}

void buildFunction(Context &c, BlockNode *block, bool get)
{
    NodeList generics;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::Lt)
    {
        buildGenericTags(c, generics, true);
        c.scanner.consume(Token::Type::Gt, false);
    }

    auto name = CommonParser::name(c, false);

    auto n = new FuncNode(name->location(), name);
    block->push_back(n);

    n->genericTags = generics;

    c.scanner.match(Token::Type::LeftParen, false);
    c.scanner.match(Token::Type::RightParen, true);

    c.scanner.match(Token::Type::Colon, true);

    n->type = TypeParser::build(c, true);

    tok = c.scanner.next(false);
    if(tok.type() == Token::Type::LeftBrace)
    {
        c.scanner.next(true);
        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
        }

        c.scanner.next(true);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

}

void DeclarationParser::build(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: buildNamespace(c, block, true); break;
        case Token::Type::RwFunc: buildFunction(c, block, true); break;

        default: break;
    }
}
