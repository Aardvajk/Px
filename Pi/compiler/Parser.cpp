#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include <pcx/lexical_cast.h>

namespace
{

void codeConstruct(Context &c, Entity *block, bool get)
{
}

Entity *headerConstruct(Context &c, Entity *block, Entity::Type type, bool get)
{
    auto name = c.scanner.match(Token::Type::StringLiteral, get);

    Entity *e = new Entity(type, name.location());
    block->entities.push_back(e);

    e->properties["name"] = name.text();

    auto tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Colon)
    {
        auto size = c.scanner.match(Token::Type::IntLiteral, true);
        e->properties["size"] = pcx::lexical_cast<std::size_t>(size.text());

        c.scanner.next(true);
    }

    return e;
}

void varConstruct(Context &c, Entity *block, Entity::Type type, bool get)
{
    auto v = headerConstruct(c, block, type, get);

    if(!v->property("size"))
    {
        throw Error("variable missing size - ", v->property("name").to<std::string>());
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void funcConstruct(Context &c, Entity *block, bool get)
{
    auto f = headerConstruct(c, block, Entity::Type::Func, get);

    if(!f->property("size"))
    {
        throw Error(f->location, "function missing return size - ", f->property("name").to<std::string>());
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
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

        default: throw Error(tok.location(), "invalid syntax - ", tok.text());
    }
}

}

EntityPtr Parser::build(Context &c)
{
    EntityPtr block = new Entity(Entity::Type::Block, { });

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block.get(), false);
    }

    return block;
}
