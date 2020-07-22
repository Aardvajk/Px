#include "Lexer.h"

#include "framework/Error.h"

#include "scanner/Source.h"
#include "scanner/Token.h"

#include <pcx/str.h>

#include <cctype>
#include <vector>

namespace
{

struct Reserved
{
    const char *text;
    Token::Type type;
};

static const std::vector<Reserved> piReserved =
{
    { "func", Token::Type::RwFunc },
    { "var", Token::Type::RwVar },
    { "arg", Token::Type::RwArg }
};

Source::Char skip(Source &source)
{
    auto c = source.get();
    while(std::isspace(c))
    {
        c = source.get();
    }

    return c;
}

Token::Type reserved(const std::vector<Reserved> &words, const std::string &text)
{
    for(auto &w: words)
    {
        if(w.text == text)
        {
            return w.type;
        }
    }

    return Token::Type::Id;
}

Token::Type reserved(Lexer::Mode mode, const std::string &text)
{
    switch(mode)
    {
        case Lexer::Mode::Pi: return reserved(piReserved, text);

        default: return Token::Type::Id;
    }
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

Token Lexer::next(Mode mode, Source &source)
{
    auto ch = skip(source);
    auto loc = source.location();

    if(!ch) return Token(Token::Type::Eof, loc, "EOF");

    if(ch == ':') return Token(Token::Type::Colon, loc, ch);
    if(ch == ';') return Token(Token::Type::Semicolon, loc, ch);
    if(ch == '&') return Token(Token::Type::Amp, loc, ch);

    if(ch == '{') return Token(Token::Type::LeftBrace, loc, ch);
    if(ch == '}') return Token(Token::Type::RightBrace, loc, ch);
    if(ch == '(') return Token(Token::Type::LeftParen, loc, ch);
    if(ch == ')') return Token(Token::Type::RightParen, loc, ch);

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
        return Token(reserved(mode, s), loc, s);
    }

    if(std::isdigit(ch))
    {
        std::string s;
        while(std::isdigit(ch))
        {
            s += char(ch);
            ch = source.get();
        }

        source.unget(ch);
        return Token(Token::Type::IntLiteral, loc, s);
    }

    throw Error(loc, pcx::str("invalid character - ", char(ch)));
}
