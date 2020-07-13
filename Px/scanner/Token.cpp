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

        default: return "(invalid)";
    }
}
