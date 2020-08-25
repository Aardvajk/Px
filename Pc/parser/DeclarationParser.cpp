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

void buildGenerics(Context &c, NodeList &container, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);
    container.push_back(new GenericNode(tok.location(), tok.text()));

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Comma)
    {
        buildGenerics(c, container, true);
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

    if(c.parseInfo.containers.back() != Sym::Type::Namespace && c.parseInfo.containers.back() != Sym::Type::Class)
    {
        throw Error(tok.location(), "invalid function - ", tok.text());
    }

    NodeList generics;

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Lt)
    {
        buildGenerics(c, generics, true);
        c.scanner.consume(Token::Type::Gt, false);
    }

    auto name = CommonParser::name(c, false);

    auto n = new FuncNode(name->location(), name);
    block->push_back(n);

    n->generics = generics;

    c.scanner.match(Token::Type::LeftParen, false);

    tok = c.scanner.next(true);
    if(tok.type() != Token::Type::RightParen)
    {
        buildArgs(c, n->args, false);
    }

    c.scanner.match(Token::Type::RightParen, false);

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Colon)
    {
        n->returnType = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
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

    auto node = new VarNode(name->location(), name);
    block->push_back(node);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        node->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::Assign)
    {
        if(c.parseInfo.containers.back() == Sym::Type::Class)
        {
            throw Error(name->location(), "members cannot have initialisers - ", name->description());
        }

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

void buildClass(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwClass, get);

    if(c.parseInfo.containers.back() != Sym::Type::Namespace && c.parseInfo.containers.back() != Sym::Type::Class)
    {
        throw Error(tok.location(), "invalid class - ", tok.text());
    }

    NodeList generics;

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Lt)
    {
        buildGenerics(c, generics, true);
        c.scanner.consume(Token::Type::Gt, false);
    }

    auto name = CommonParser::name(c, false);

    auto node = new ClassNode(tok.location(), name);
    block->push_back(node);

    node->generics = generics;

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        auto cg = pcx::scoped_push(c.parseInfo.containers, Sym::Type::Class);

        node->body = CommonParser::blockContents(c, tok.location(), false);
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
        case Token::Type::RwNamespace: buildNamespace(c, block, false); break;
        case Token::Type::RwFunc: buildFunction(c, block, false); break;
        case Token::Type::RwVar: buildVar(c, block, false); break;
        case Token::Type::RwClass: buildClass(c, block, false); break;

        default: break;
    }
}
