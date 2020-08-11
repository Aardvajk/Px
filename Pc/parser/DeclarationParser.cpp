#include "DeclarationParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"
#include "parser/TypeParser.h"
#include "parser/ExprParser.h"

#include <pcx/scoped_push.h>

namespace
{

void buildNamespace(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwNamespace, get);

    auto name = CommonParser::name(c, true);

    if(c.parseInfo.containers.back() != Sym::Type::Namespace)
    {
        throw Error(name->location(), "invalid namespace - ", name->description());
    }

    auto n = new NamespaceNode(tok.location(), name);
    block->push_back(n);

    auto cg = pcx::scoped_push(c.parseInfo.containers, Sym::Type::Namespace);

    n->body = CommonParser::blockContents(c, c.scanner.token().location(), false);
}

void buildGenericTags(Context &c, NodeList &container, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);
    container.push_back(new GenericTagNode(tok.location(), tok.text()));

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Comma)
    {
        buildGenericTags(c, container, true);
    }
}

void buildArgs(Context &c, NodeList &container, bool get)
{
    std::string name;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::Id)
    {
        name = tok.text();
        c.scanner.next(true);
    }

    auto v = new VarNode(tok.location(), new IdNode(tok.location(), { }, name));
    container.push_back(v);

    c.scanner.match(Token::Type::Colon, false);
    v->type = TypeParser::build(c, true);

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildArgs(c, container, true);
    }
}

void buildFunction(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwFunc, get);

    NodeList generics;
    if(c.parseInfo.containers.back() != Sym::Type::Namespace)
    {
        throw Error(tok.location(), "invalid function - ", tok.text());
    }

    tok = c.scanner.next(true);

    if(tok.type() == Token::Type::Lt)
    {
        buildGenericTags(c, generics, true);
        c.scanner.consume(Token::Type::Gt, false);
    }

    auto name = CommonParser::name(c, false);

    auto n = new FuncNode(name->location(), name);
    block->push_back(n);

    n->genericTags = generics;

    c.scanner.match(Token::Type::LeftParen, false);

    tok = c.scanner.next(true);
    if(tok.type() != Token::Type::RightParen)
    {
        buildArgs(c, n->args, false);
    }

    c.scanner.match(Token::Type::RightParen, false);
    c.scanner.match(Token::Type::Colon, true);

    n->returnType = TypeParser::build(c, true);

    tok = c.scanner.next(false);
    if(tok.type() == Token::Type::LeftBrace)
    {
        auto scope = new ScopeNode(tok.location());
        n->body = scope;

        auto cg = pcx::scoped_push(c.parseInfo.containers, Sym::Type::Func);

        scope->body = CommonParser::blockContents(c, tok.location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void buildVarImp(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    if(c.parseInfo.containers.back() != Sym::Type::Namespace)
    {
        throw Error(name->location(), "invalid var - ", name->description());
    }

    auto node = new VarNode(name->location(), name);
    block->push_back(node);

    c.scanner.match(Token::Type::Colon, false);

    node->type = TypeParser::build(c, true);

    if(c.scanner.token().type() == Token::Type::Assign)
    {
        node->value = ExprParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildVarImp(c, block, true);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void buildVar(Context &c, BlockNode *block, bool get)
{
    c.scanner.match(Token::Type::RwVar, get);
    buildVarImp(c, block, true);
}

}

void DeclarationParser::build(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: buildNamespace(c, block, false); break;
        case Token::Type::RwFunc: buildFunction(c, block, false); break;
        case Token::Type::RwVar: buildVar(c, block, false); break;

        default: break;
    }
}
