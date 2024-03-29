#ifndef LEXER_H
#define LEXER_H

#include <cstdint>
#include <string>

class Token;
class Source;

namespace Lexer
{

enum class Mode : std::uint8_t
{
    Pi, Pc, Map
};

Token next(Mode mode, Source &source);

std::string encodeString(const std::string &text);
std::string decodeString(const std::string &text);

}

#endif // LEXER_H
