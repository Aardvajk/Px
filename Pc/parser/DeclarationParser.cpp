#include "DeclarationParser.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/FuncNode.h"

#include "parser/CommonParser.h"
#include "parser/TypeParser.h"

namespace
{

void buildFunction(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    auto n = new FuncNode(name->location(), name);
    block->push_back(n);

    c.scanner.match(Token::Type::LeftParen, false);
    c.scanner.match(Token::Type::RightParen, true);

    c.scanner.match(Token::Type::Colon, true);

    n->type = TypeParser::build(c, true);

    auto tok = c.scanner.next(false);
    if(tok.type() == Token::Type::LeftBrace)
    {
        c.scanner.next(true);
        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
        }

        c.scanner.next(true);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

}

void DeclarationParser::build(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwFunc: buildFunction(c, block, true); break;

        default: break;
    }
}
