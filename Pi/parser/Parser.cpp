#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"
#include "application//Code.h"

#include <pcx/lexical_cast.h>

namespace
{

void pushNumericLiteralConstruct(Context &c, Token::Type type, Entity *e, bool get)
{
    c.scanner.match(Token::Type::LeftParen, get);

    e->properties["pushtype"] = std::string("numeric");
    e->properties["valuetype"] = type;

    auto val = c.scanner.match(Token::Type::IntLiteral, true).text();

    switch(type)
    {
        case Token::Type::RwInt: e->properties["value"] = pcx::lexical_cast<int>(val); break;

        default: break;
    }

    c.scanner.match(Token::Type::RightParen, true);
}

void pushAddrOfConstruct(Context &c, Entity *e, bool get)
{
    auto id = c.scanner.match(Token::Type::StringLiteral, get);

    e->properties["pushtype"] = std::string("addrof");
    e->properties["target"] = id.text();
}

void pushValueConstruct(Context &c, Entity *e, bool get)
{
    auto id = c.scanner.match(Token::Type::StringLiteral, get);

    e->properties["pushtype"] = std::string("value");
    e->properties["target"] = id.text();
}

void pushConstruct(Context &c, Entity *e, bool get)
{
    auto tok = c.scanner.next(get);

    switch(tok.type())
    {
        case Token::Type::RwInt: pushNumericLiteralConstruct(c, tok.type(), e, true); break;

        case Token::Type::Amp: pushAddrOfConstruct(c, e, true); break;
        case Token::Type::StringLiteral: pushValueConstruct(c, e, false); break;

        default: throw Error(tok.location(), "invalid push syntax - ", tok.text());
    }
}

void popConstruct(Context &c, Entity *e, bool get)
{
    e->properties["amount"] = pcx::lexical_cast<std::size_t>(c.scanner.match(Token::Type::IntLiteral, get).text());
}

void serviceConstruct(Context &c, Entity *e, bool get)
{
    e->properties["code"] = pcx::lexical_cast<int>(c.scanner.match(Token::Type::IntLiteral, get).text());
}

void codeConstruct(Context &c, Entity *block, bool get)
{
    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::StringLiteral)
    {
        auto e = new Entity(Entity::Type::Label, tok.location());
        block->entities.push_back(e);

        e->properties["name"] = tok.text();
        c.scanner.consume(Token::Type::Colon, true);
    }
    else
    {
        auto t = Code::fromString(tok.text());
        if(t == Code::Type::Invalid)
        {
            throw Error(tok.location(), "instruction expected - ", tok.text());
        }

        auto e = new Entity(Entity::Type::Instruction, tok.location());
        block->entities.push_back(e);

        e->properties["instruction"] = t;

        switch(t)
        {
            case Code::Type::Push: pushConstruct(c, e, true); break;
            case Code::Type::Pop: popConstruct(c, e, true); break;

            case Code::Type::Call: break;

            case Code::Type::Service: serviceConstruct(c, e, true); break;

            default: break;
        }

        c.scanner.consume(Token::Type::Semicolon, true);
    }
}

Entity *headerConstruct(Context &c, Entity *block, Entity::Type type, bool get)
{
    auto name = c.scanner.match(Token::Type::StringLiteral, get);

    auto e = new Entity(type, name.location());
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
        f->properties["defined"] = true;

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
    EntityPtr block = new Entity(Entity::Type::Block, Location());

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block.get(), false);
    }

    return block;
}
