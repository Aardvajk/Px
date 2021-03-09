#include "Lexer.h"

#include "framework/Error.h"

#include "scanner/Token.h"
#include "scanner/Source.h"

#include <pcx/str.h>

#include <cctype>
#include <vector>
#include <algorithm>

namespace
{

struct EncodedChar
{
    char values[2];
};

const std::vector<EncodedChar> encodedChars
{
    { '\n', 'n' },
    { '\t', 't' },
    { '\0', '0' },
    { '\'', '\'' },
    { '\"', '\"' }
};

struct Reserved
{
    const char *text;
    Token::Type type;
};

const std::vector<Reserved> piReserved =
{
    { "func", Token::Type::RwFunc },
    { "var", Token::Type::RwVar },
    { "arg", Token::Type::RwArg }
};

template<int K, int V> char searchEncodedChar(char ch)
{
    auto i = std::find_if(encodedChars.begin(), encodedChars.end(), [ch](EncodedChar v){ return v.values[K] == ch; });
    if(i != encodedChars.end())
    {
        return i->values[V];
    }

    return ch;
}

char encodedChar(char ch)
{
    return searchEncodedChar<0, 1>(ch);
}

char decodedChar(char ch)
{
    return searchEncodedChar<1, 0>(ch);
}

Source::Char skip(Source &source)
{
    auto ch = source.get();
    while(std::isspace(ch))
    {
        ch = source.get();
    }

    return ch;
}

Token charConstant(Source &source, Location location)
{
    auto v = source.get();
    if(v == '\\')
    {
        v = decodedChar(source.get());
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

    while(ch != '\"')
    {
        if(!ch || ch == '\n')
        {
            throw Error(location, "non-terminated string constant");
        }

        s += char(ch);

        ch = source.get();
    }

    ch = skip(source);
    if(ch == '\"')
    {
        return stringConstant(source, location, s);
    }

    source.unget(ch);
    return Token(Token::Type::StringLiteral, location, Lexer::decodeString(s));
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

}

Token Lexer::next(Mode mode, Source &source)
{
    auto ch = skip(source);
    auto loc = source.location();

    if(!ch) return Token(Token::Type::Eof, loc, "EOF");

    if(ch == ':') return Token(Token::Type::Colon, loc, ch);
    if(ch == ';') return Token(Token::Type::Semicolon, loc, ch);

    if(ch == '{') return Token(Token::Type::LeftBrace, loc, ch);
    if(ch == '}') return Token(Token::Type::RightBrace, loc, ch);

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

    throw Error(loc, "invalid character - ", char(ch));
}

std::string Lexer::encodeString(const std::string &text)
{
    std::string s;

    for(std::size_t i = 0; i < text.length(); ++i)
    {
        auto ch = encodedChar(text[i]);

        if(ch != text[i])
        {
            s += pcx::str("\\", ch);
        }
        else
        {
            s += ch;
        }
    }

    return s;
}

std::string Lexer::decodeString(const std::string &text)
{
    std::string s;

    for(std::size_t i = 0; i < text.length(); ++i)
    {
        auto ch = text[i];

        if(ch == '\\' && i < text.length() - 1)
        {
            ch = decodedChar(text[++i]);
        }

        s += ch;
    }

    return s;
}
