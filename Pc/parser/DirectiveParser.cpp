#include "DirectiveParser.h"

#include "application/Context.h"

#include "parser/Parser.h"

void DirectiveParser::buildInclude(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::StringLiteral, get);

    auto path = tok.text();
    if(!c.sources.contains(path))
    {
        c.open(path);

        c.scanner.next(true);
        while(c.scanner.token().type() != Token::Type::Eof)
        {
            Parser::construct(c, block, false);
        }

        c.scanner.pop();
    }

    c.scanner.consume(Token::Type::Semicolon, true);
}
