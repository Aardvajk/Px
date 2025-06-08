#include "Scanner.h"

#include "framework/Error.h"

Scanner::Scanner()
{
}

void Scanner::push(Source *source)
{
    state.push_back({ source, Token() });
}

void Scanner::pop()
{
    state.pop_back();
}

Token Scanner::next(bool get)
{
    if(get)
    {
        state.back().tok;// = Lexer::next(mode, *state.back().src);
        if(state.back().tok.type() == Token::Type::Invalid)
        {
            throw Error(state.back().tok.location(), "invalid token - ", state.back().tok.text());
        }
    }

    return state.back().tok;
}

Token Scanner::match(Token::Type type, bool get)
{
    auto tok = next(get);
    if(tok.type() != type)
    {
        throw Error(tok.location(), Token::toString(type), " expected - ", tok.text());
    }

    return tok;
}

Token Scanner::consume(Token::Type type, bool get)
{
    match(type, get);
    return next(true);
}

Token Scanner::token() const
{
    return state.back().tok;
}
