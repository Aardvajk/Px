#ifndef LEXER_H
#define LEXER_H

class Token;
class Source;

namespace Lexer
{

enum class Mode
{
    Pi
};

Token next(Mode mode, Source &source);

}

#endif // LEXER_H
