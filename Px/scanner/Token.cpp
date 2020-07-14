#include "Token.h"

Token::Token() : t(Type::Invalid)
{
}

Token::Token(Type type, Location location, std::string text) : t(type), n(location), s(std::move(text))
{
}

Token::Token(Type type, Location location, Source::Char ch) : t(type), n(location), s(1, char(ch))
{
}

const char *Token::toString(Type v)
{
    switch(v)
    {
        case Type::Id: return "identifier";
        case Type::StringLiteral: return "string literal";
        case Type::IntLiteral: return "int literal";
        case Type::Colon: return "colon";
        case Type::Semicolon: return "semicolon";
        case Type::LeftBrace: return "left brace";
        case Type::RightBrace: return "right brace";
        case Type::RwFunc: return "func";
        case Type::RwVar: return "var";
        case Type::RwArg: return "arg";

        default: return "(invalid)";
    }
}
