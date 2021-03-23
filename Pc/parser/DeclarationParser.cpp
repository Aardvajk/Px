#include "DeclarationParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"
#include "parser/TypeParser.h"

#include <pcx/scoped_push.h>

namespace
{

void namespaceConstruct(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    if(c.parseInfo.containers.back() != Sym::Type::Namespace)
    {
        throw Error(name->location(), "invalid namespace - ", name->description());
    }

    auto ns = new NamespaceNode(name->location(), name);
    block->push_back(ns);

    auto sc = pcx::scoped_push(c.parseInfo.containers, Sym::Type::Namespace);

    ns->body = CommonParser::blockContents(c, name->location(), false);
}

void funcConstruct(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    if(c.parseInfo.containers.back() != Sym::Type::Namespace)
    {
        throw Error(name->location(), "invalid function - ", name->description());
    }

    auto func = new FuncNode(name->location(), name);
    block->push_back(func);

    c.scanner.match(Token::Type::LeftParen, false);
    c.scanner.consume(Token::Type::RightParen, true);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        func->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        auto sc = pcx::scoped_push(c.parseInfo.containers, Sym::Type::Func);

        auto scope = new ScopeNode(c.scanner.token().location());
        func->body = scope;

        scope->body = CommonParser::blockContents(c, c.scanner.token().location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void classConstruct(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    if(c.parseInfo.containers.back() == Sym::Type::Func)
    {
        throw Error(name->location(), "invalid class - ", name->description());
    }

    auto cn = new ClassNode(name->location(), name);
    block->push_back(cn);

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        auto sc = pcx::scoped_push(c.parseInfo.containers, Sym::Type::Class);

        cn->body = CommonParser::blockContents(c, c.scanner.token().location(), false);
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
        case Token::Type::RwNamespace: namespaceConstruct(c, block, true); break;
        case Token::Type::RwFunc: funcConstruct(c, block, true); break;
        case Token::Type::RwClass: classConstruct(c, block, true); break;

        default: break;
    }
}
