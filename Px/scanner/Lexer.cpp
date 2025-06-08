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

}

Token Lexer::next(Source &source)
{
    auto ch = skip(source);
    auto loc = source.location();

    if(!ch) return Token(Token::Type::Eof, loc, "EOF");

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

