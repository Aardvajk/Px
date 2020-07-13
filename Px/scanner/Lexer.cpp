#include "Lexer.h"

#include "framework/Error.h"

#include "scanner/Source.h"
#include "scanner/Token.h"

#include <pcx/str.h>

#include <cctype>

namespace
{

Source::Char skip(Source &source)
{
    auto c = source.get();
    while(std::isspace(c))
    {
        c = source.get();
    }

    return c;
}

}

Token Lexer::next(Mode mode, Source &source)
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
