#include "FuncParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/TypeParser.h"
#include "parser/ExprParser.h"
#include "parser/CommonParser.h"

namespace
{

void buildVar(Context &c, BlockNode *block, bool get)
{
    c.scanner.match(Token::Type::RwVar, get);

    auto name = c.scanner.match(Token::Type::Id, true);

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

void buildReturn(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwReturn, get);

    auto node = new ReturnNode(tok.location());
    block->push_back(node);

    tok = c.scanner.next(true);
    if(tok.type() != Token::Type::Semicolon)
    {
        node->expr = ExprParser::build(c, false);
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void buildIf(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwIf, get);

    auto node = new IfNode(tok.location());
    block->push_back(node);

    node->expr = CommonParser::parenthesisedExprList(c, true);
    node->body = CommonParser::scopeContents(c, c.scanner.token().location(), false);

    if(c.scanner.token().type() == Token::Type::RwElse)
    {
        node->elseBody = CommonParser::scopeContents(c, c.scanner.token().location(), true);
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
        case Token::Type::RwVar: buildVar(c, block, false); break;
        case Token::Type::RwReturn: buildReturn(c, block, false); break;
        case Token::Type::RwIf: buildIf(c, block, false); break;

        default: buildExpr(c, block, false);
    }
}
