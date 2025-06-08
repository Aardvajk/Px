#include "Lexer.h"

#include "framework/Error.h"

#include "scanner/Token.h"
#include "scanner/Source.h"

#include <pcx/str.h>

namespace
{

Source::Char skip(Source &source)
{
    auto ch = source.get();
    while(std::isspace(ch))
    {
        ch = source.get();
    }

    if(ch == '/')
    {
        ch = source.get();
        if(ch == '/')
        {
            ch = source.get();
            while(ch && ch != '\n')
            {
                ch = source.get();
            }

            return skip(source);
        }
        else
        {
            source.unget(ch);
            return '/';
        }
    }

    return ch;
}

Source::Char translateEscapeChar(Source::Char ch)
{
    switch(ch)
    {
        case 'n': return '\n';
        case 't': return '\t';
        case '0': return '\0';
        case '\"': return '\"';
        case '\'': return '\'';

        default: return ch;
    }
}

Token charConstant(Source &source, Location location)
{
    auto v = source.get();
    if(v == '\\')
    {
        v = translateEscapeChar(source.get());
    }

    auto ch = source.get();
    if(ch != '\'')
    {
        throw Error(location, "non-terminated character");
    }

    return Token(Token::Type::CharLiteral, location, v);
}

Token stringConstant(Source &source, Location location, std::string &s)
{
    auto ch = source.get();
    bool translated = false;

    if(ch == '\\')
    {
        ch = translateEscapeChar(source.get());
        translated = true;
    }

    while(ch != '\"' || translated)
    {
        if(!translated && (!ch || ch == '\n'))
        {
            throw Error(location, "non-terminated string constant");
        }

        s += char(ch);

        ch = source.get();

        translated = false;
        if(ch == '\\')
        {
            ch = translateEscapeChar(source.get());
            translated = true;
        }
    }

    ch = skip(source);
    if(ch == '\"')
    {
        return stringConstant(source, location, s);
    }

    source.unget(ch);
    return Token(Token::Type::StringLiteral, location, s);
}

}

Token Lexer::next(Source &source)
{
    auto ch = skip(source);
    auto loc = source.location();

    if(!ch) return Token(Token::Type::Eof, loc, "EOF");

    if(ch == '\'')
    {
        return charConstant(source, loc);
    }

    if(ch == '\"')
    {
        std::string s;
        return stringConstant(source, loc, s);
    }

    if(std::isalpha(ch) || ch == '_')
    {
        std::string s;
        while(std::isalnum(ch) || ch == '_')
        {
            s += char(ch);
            ch = source.get();
        }

        source.unget(ch);
        return Token(Token::Type::Id, loc, s);
    }

    throw Error(loc, pcx::str("invalid character - ", char(ch)));
}

