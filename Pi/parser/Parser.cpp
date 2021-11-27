#include "Parser.h"

#include "framework/Error.h"

#include "common/Primitives.h"

#include "application/Context.h"
#include "application//Code.h"

#include <pcx/lexical_cast.h>
#include <pcx/datastream.h>

namespace
{

template<typename T> std::vector<char> primitiveToBytes(T value)
{
    pcx::data_ostringstream os;
    os << value;

    return os.data();
}

void pushNumericLiteralConstruct(Context &c, Primitive::Type type, Entity *e, bool get)
{
    c.scanner.match(Token::Type::LeftParen, get);

    e->properties["pushtype"] = std::string("numeric");
    e->properties["valuetype"] = type;

    auto val = c.scanner.match(Token::Type::IntLiteral, true).text();

    switch(type)
    {
        case Primitive::Type::Char: e->properties["value"] = static_cast<char>(pcx::lexical_cast<int>(val)); break;
        case Primitive::Type::Int: e->properties["value"] = pcx::lexical_cast<int>(val); break;
        case Primitive::Type::ULong: e->properties["value"] = pcx::lexical_cast<std::size_t>(val); break;

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

    if(tok.type() == Token::Type::Id)
    {
        auto type = Primitive::fromString(tok.text());
        if(type != Primitive::Type::Null && type != Primitive::Type::Invalid)
        {
            pushNumericLiteralConstruct(c, type, e, true);
            return;
        }
    }

    switch(tok.type())
    {
        case Token::Type::Amp: pushAddrOfConstruct(c, e, true); break;
        case Token::Type::StringLiteral: pushValueConstruct(c, e, false); break;

        default: throw Error(tok.location(), "invalid push syntax - ", tok.text());
    }
}

void jumpConstruct(Context &c, Entity *e, bool get)
{
    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::Id && tok.text() == "ifz")
    {
        e->properties["ifz"] = true;
        c.scanner.next(true);
    }

    e->properties["target"] = c.scanner.match(Token::Type::StringLiteral, false).text();
}

void amountConstruct(Context &c, Entity *e, bool get)
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

            case Code::Type::Call: break;
            case Code::Type::Jump: jumpConstruct(c, e, true); break;

            case Code::Type::Pop:
            case Code::Type::Load:
            case Code::Type::Store:
            case Code::Type::AllocS:
            case Code::Type::IncrS: amountConstruct(c, e, true); break;

            case Code::Type::Service: serviceConstruct(c, e, true); break;

            default: break;
        }

        c.scanner.consume(Token::Type::Semicolon, true);
    }
}

void flagsConstruct(Context &c, Object::Entity::Flags &flags, bool get)
{
    auto tok = c.scanner.next(true);

    if(tok.text() == "autogen")
    {
        flags |= Object::Entity::Flag::Autogen;
    }
    else if(tok.text() == "external")
    {
        flags |= Object::Entity::Flag::External;
    }
    else
    {
        throw Error(tok.location(), "invalid flag - ", tok.text());
    }

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Comma)
    {
        flagsConstruct(c, flags, true);
    }
}

Entity *headerConstruct(Context &c, Entity *block, Entity::Type type, bool get)
{
    Object::Entity::Flags flags;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::LeftSub)
    {
        flagsConstruct(c, flags, true);
        c.scanner.consume(Token::Type::RightSub, false);
    }

    auto name = c.scanner.match(Token::Type::StringLiteral, false);

    auto e = new Entity(type, name.location());
    block->entities.push_back(e);

    e->properties["name"] = name.text();
    e->properties["flags"] = flags;

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Colon)
    {
        e->properties["size"] = pcx::lexical_cast<std::size_t>(c.scanner.match(Token::Type::IntLiteral, true).text());
        c.scanner.next(true);
    }

    return e;
}

void byteConstruct(Context &c, std::vector<char> &v, bool get)
{
    auto tok = c.scanner.match(Token::Type::IntLiteral, get);
    v.push_back(static_cast<char>(pcx::lexical_cast<int>(tok.text())));

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Comma)
    {
        byteConstruct(c, v, true);
    }
}

void varConstruct(Context &c, Entity *block, Entity::Type type, bool get)
{
    auto e = headerConstruct(c, block, type, get);

    auto tok = c.scanner.next(false);
    if(tok.type() == Token::Type::Assign)
    {
        if(e->property("flags").to<Object::Entity::Flags>() & Object::Entity::Flag::External)
        {
            throw Error(e->location, "initialising an external - ", e->property("name").to<std::string>());
        }

        std::vector<char> v;

        tok = c.scanner.next(true);

        auto primitive = Primitive::Type::Invalid;
        if(tok.type() == Token::Type::Id)
        {
            primitive = Primitive::fromString(tok.text());
        }

        if(primitive != Primitive::Type::Null && primitive != Primitive::Type::Invalid)
        {
            c.scanner.match(Token::Type::LeftParen, true);

            auto val = c.scanner.match(Token::Type::IntLiteral, true);

            switch(primitive)
            {
                case Primitive::Type::Char: v = primitiveToBytes(static_cast<char>(pcx::lexical_cast<int>(val.text()))); break;
                case Primitive::Type::Int: v = primitiveToBytes(pcx::lexical_cast<int>(val.text())); break;
                case Primitive::Type::ULong: v = primitiveToBytes(static_cast<std::size_t>(pcx::lexical_cast<int>(val.text()))); break;

                default: break;
            }


            c.scanner.consume(Token::Type::RightParen, true);
        }
        else if(tok.type() == Token::Type::StringLiteral)
        {
            auto s = tok.text();
            for(auto i: s)
            {
                v.push_back(i);
            }

            v.push_back(0);
            c.scanner.next(true);
        }
        else
        {
            c.scanner.match(Token::Type::LeftBrace, false);

            byteConstruct(c, v, true);

            c.scanner.consume(Token::Type::RightBrace, false);
        }

        if(e->property("size"))
        {
            if(v.size() != e->property("size").to<std::size_t>())
            {
                throw Error(e->location, "mismatched data size - ", e->property("name").to<std::string>());
            }
        }
        else
        {
            e->properties["size"] = v.size();
        }

        e->properties["bytes"] = v;
    }

    if(!e->property("size"))
    {
        throw Error(e->location, "global missing size - ", e->property("name").to<std::string>());
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void funcConstruct(Context &c, Entity *block, bool get)
{
    auto f = headerConstruct(c, block, Entity::Type::Func, get);

    if(!f->property("size"))
    {
        throw Error(f->location, "function return size missing - ", f->property("name").to<std::string>());
    }

    auto tok = c.scanner.next(false);
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
