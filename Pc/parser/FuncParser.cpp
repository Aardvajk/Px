#include "FuncParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"
#include "parser/ExprParser.h"

namespace
{

void buildExpression(Context &c, BlockNode *block, bool get)
{
    auto n = new ExprNode(c.scanner.token().location());
    block->push_back(n);

    n->expr = ExprParser::build(c, false);

    c.scanner.consume(Token::Type::Semicolon, false);
}

void buildAnonymousScope(Context &c, BlockNode *block, bool get)
{
    auto n = new ScopeNode(c.scanner.token().location());
    block->push_back(n);

    n->body = CommonParser::blockContents(c, get);
}

}

void FuncParser::build(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);

    if(c.containers.back() != Sym::Type::Func)
    {
        throw Error(tok.location(), "declaration expected - ", tok.text());
    }

    switch(tok.type())
    {
        case Token::Type::LeftBrace: buildAnonymousScope(c, block, false); break;

        default: buildExpression(c, block, false);
    }
}
