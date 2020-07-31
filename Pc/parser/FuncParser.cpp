#include "FuncParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/TypeParser.h"
#include "parser/ExprParser.h"

namespace
{

void buildVar(Context &c, BlockNode *block, bool get)
{
    auto name = c.scanner.match(Token::Type::Id, get);

    auto var = new VarNode(name.location(), new IdNode(name.location(), { }, name.text()));
    block->push_back(var);

    c.scanner.match(Token::Type::Colon, true);

    var->type = TypeParser::build(c, true);

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildVar(c, block, true);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

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
        case Token::Type::RwVar: buildVar(c, block, true); break;

        default: buildExpr(c, block, false);
    }
}
