#ifndef LEXER_H
#define LEXER_H

#include <cstdint>

class Token;
class Source;

namespace Lexer
{

enum class Mode : std::uint8_t
{
    Pi
};

Token next(Mode mode, Source &source);

}

#endif // LEXER_H
