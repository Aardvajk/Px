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

    throw Error(loc, "invalid character - ", char(ch));
}
