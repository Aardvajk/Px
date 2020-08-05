#include "DeclarationParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"
#include "parser/TypeParser.h"

#include <pcx/scoped_push.h>

namespace
{

void buildNamespace(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    if(c.parseInfo.containers.back() != Sym::Type::Namespace)
    {
        throw Error(name->location(), "namespace invalid - ", name->description());
    }

    auto n = new NamespaceNode(name->location(), name);
    block->push_back(n);

    auto cp = pcx::scoped_push(c.parseInfo.containers, Sym::Type::Namespace);

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
    NodeList generics;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::Lt)
    {
        buildGenericTags(c, generics, true);
        c.scanner.consume(Token::Type::Gt, false);
    }

    auto name = CommonParser::name(c, false);

    if(c.parseInfo.containers.back() != Sym::Type::Namespace)
    {
        throw Error(name->location(), "function invalid - ", name->description());
    }

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

    n->type = TypeParser::build(c, true);

    tok = c.scanner.next(false);
    if(tok.type() == Token::Type::LeftBrace)
    {
        auto scope = new ScopeNode(tok.location());
        n->body = scope;

        auto cp = pcx::scoped_push(c.parseInfo.containers, Sym::Type::Func);

        scope->body = CommonParser::blockContents(c, tok.location(), false);
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
        case Token::Type::RwNamespace: buildNamespace(c, block, true); break;
        case Token::Type::RwFunc: buildFunction(c, block, true); break;

        default: break;
    }
}