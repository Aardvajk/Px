#include "Token.h"

Token::Token() : t(Type::Invalid)
{
}

Token::Token(Type type, Location location, Source::Char ch) : t(type), n(location), s(std::string(1, char(ch)))
{
}

Token::Token(Type type, Location location, std::string text) : t(type), n(location), s(std::move(text))
{
}

const char *Token::toString(Type v)
{
    switch(v)
    {
        case Type::Id: return "identifier";

        case Type::IntLiteral: return "number";

        case Type::Colon: return "colon";
        case Type::Semicolon: return "semicolon";
        case Type::Comma: return "comma";
        case Type::Dot: return "dot";

        case Type::LeftBrace: return "left brace";
        case Type::RightBrace: return "right brace";
        case Type::LeftParen: return "left bracket";
        case Type::RightParen: return "right bracket";
        case Type::LeftSub: return "left subscript";
        case Type::RightSub: return "right subscript";

        default: return "(unknown)";
    }
}
