#ifndef TOKEN_H
#define TOKEN_H

#include "framework/Location.h"

#include "scanner/Source.h"

#include <string>

class Token
{
public:
    enum class Type
    {
        Id,
        CharLiteral, StringLiteral, IntLiteral,
        Colon, Semicolon, Comma,
        LeftBrace, RightBrace, LeftParen, RightParen, LeftSub, RightSub,
        RwFunc, RwVar, RwArg,
        Eof, Invalid
    };

    Token();
    Token(Type type, Location location, Source::Char ch);
    Token(Type type, Location location, std::string text);

    Type type() const { return t; }
    Location location() const { return n; }
    std::string text() const { return s; }

    operator bool() const { return t != Type::Invalid; }

    static const char *toString(Type v);

private:
    Type t;
    Location n;
    std::string s;
};

#endif // TOKEN_H
