#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "parser/Code.h"

#include <pcx/lexical_cast.h>

namespace
{

void codeConstruct(Context &c, Entity *block, bool get)
{
    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::StringLiteral)
    {
        auto e = new Entity(Entity::Type::Label);
        block->entities.push_back(e);

        e->properties["name"] = tok.text();
        c.scanner.consume(Token::Type::Colon, true);
    }
    else
    {
        auto t = Code::fromString(tok.text());
        switch(t)
        {
            default: throw Error(tok.location(), "instruction expected - ", tok.text());
        }
    }
}

Entity *headerConstruct(Context &c, Entity *block, Entity::Type type, bool get)
{
    auto name = c.scanner.match(Token::Type::StringLiteral, get);

    auto e = new Entity(type);
    block->entities.push_back(e);

    e->properties["name"] = name.text();

    c.scanner.match(Token::Type::Colon, true);
    e->properties["size"] = pcx::lexical_cast<std::size_t>(c.scanner.match(Token::Type::IntLiteral, true).text());

    return e;
}

void varConstruct(Context &c, Entity *block, Entity::Type type, bool get)
{
    headerConstruct(c, block, type, get);
    c.scanner.consume(Token::Type::Semicolon, true);
}

void funcConstruct(Context &c, Entity *block, bool get)
{
    auto f = headerConstruct(c, block, Entity::Type::Func, get);

    auto tok = c.scanner.next(true);
    if(tok.type() == Token::Type::LeftBrace)
    {
        c.scanner.next(true);
        while(c.scanner.token().type() == Token::Type::RwArg)
        {
            varConstruct(c, f, Entity::Type::Arg, true);
        }

        while(c.scanner.token().type() == Token::Type::RwVar)
        {
            varConstruct(c, f, Entity::Type::Var, true);
        }

        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
            codeConstruct(c, f, false);
        }

        c.scanner.next(true);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void construct(Context &c, Entity *block, bool get)
{
    auto tok = c.scanner.next(get);

    switch(tok.type())
    {
        case Token::Type::RwVar: varConstruct(c, block, Entity::Type::Global, true); break;
        case Token::Type::RwFunc: funcConstruct(c, block, true); break;

        default: throw Error(tok.location(), "construct expected - ", tok.text());
    }
}

}

EntityPtr Parser::build(Context &c)
{
    EntityPtr block = new Entity(Entity::Type::Block);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block.get(), false);
    }

    return block;
}
