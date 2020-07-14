#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include <pcx/lexical_cast.h>

namespace
{

void funcConstruct(Context &c, Entity *block, bool get)
{
    auto name = c.scanner.match(Token::Type::StringLiteral, get);

    auto f = new Entity(Entity::Type::Func);
    block->entities.push_back(f);

    f->properties["name"] = name.text();

    c.scanner.match(Token::Type::Colon, true);
    f->properties["size"] = pcx::lexical_cast<std::size_t>(c.scanner.match(Token::Type::IntLiteral, true).text());

    auto tok = c.scanner.next(true);
    if(tok.type() == Token::Type::LeftBrace)
    {
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
