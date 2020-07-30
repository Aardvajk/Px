#include "FuncParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/ExprParser.h"

namespace
{

void buildExpr(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);

    auto expr = new ExprNode(tok.location());
    block->push_back(expr);

    expr->expr = ExprParser::build(c, false);

    c.scanner.consume(Token::Type::Semicolon, false);
}

}

void FuncParser::build(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);

    if(c.parseInfo.containers.back() != Sym::Type::Func)
    {
        throw Error(tok.location(), "declaration expected - ", tok.text());
    }

    switch(tok.type())
    {
        default: buildExpr(c, block, false);
    }
}
