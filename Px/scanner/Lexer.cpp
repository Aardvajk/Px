#include "Lexer.h"

#include "framework/Error.h"

#include "scanner/Token.h"
#include "scanner/Source.h"

#include <cctype>

namespace
{

Source::Char skip(Source &source)
{
    auto ch = source.get();
    while(std::isspace(ch))
    {
        ch = source.get();
    }

    return ch;
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

    throw Error(loc, "invalid character - ", char(ch));
}
