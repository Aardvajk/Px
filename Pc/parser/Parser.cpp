#include "Parser.h"

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
    auto n = new NamespaceNode(c.scanner.token().location());
    block->push_back(n);

    n->name = CommonParser::name(c, get);

    if(c.containers.back() != Sym::Type::Namespace)
    {
        throw Error(n->name->location(), "invalid location for namespace - ", n->name->description());
    }

    auto cg = pcx::scoped_push(c.containers, Sym::Type::Namespace);

    n->body = CommonParser::blockContents(c, false);
}

void buildArgs(Context &c, NodeList &container, bool get)
{
    auto n = new VarNode(c.scanner.token().location());
    container.push_back(n);

    auto tok = c.scanner.next(get);

    std::string name;
    if(tok.type() == Token::Type::Id)
    {
        name = tok.text();
        c.scanner.next(true);
    }

    n->name = new IdNode(tok.location(), { }, name);

    c.scanner.match(Token::Type::Colon, false);
    n->type = TypeParser::build(c, true);

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildArgs(c, container, true);
    }
}

void buildFunc(Context &c, BlockNode *block, bool get)
{
    auto n = new FuncNode(c.scanner.token().location());
    block->push_back(n);

    n->name = CommonParser::name(c, get);

    if(c.containers.back() != Sym::Type::Namespace && c.containers.back() != Sym::Type::Class)
    {
        throw Error(n->name->location(), "invalid location for function - ", n->name->description());
    }

    c.scanner.consume(Token::Type::LeftParen, false);
    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        buildArgs(c, n->args, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() != Token::Type::Semicolon)
    {
        auto cg = pcx::scoped_push(c.containers, Sym::Type::Func);

        auto s = new ScopeNode(c.scanner.token().location());
        n->body = s;

        s->body = CommonParser::blockContents(c, false);
    }
    else
    {
        c.scanner.next(true);
    }
}

void buildVar(Context &c, BlockNode *block, bool get)
{
    auto n = new VarNode(c.scanner.token().location());
    block->push_back(n);

    n->name = CommonParser::name(c, get);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildVar(c, block, true);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void buildClass(Context &c, BlockNode *block, bool get)
{
    auto n = new ClassNode(c.scanner.token().location());
    block->push_back(n);

    n->name = CommonParser::name(c, get);

    if(c.containers.back() != Sym::Type::Namespace && c.containers.back() != Sym::Type::Class)
    {
        throw Error(n->name->location(), "invalid location for class - ", n->name->description());
    }

    if(c.scanner.token().type() != Token::Type::Semicolon)
    {
        auto cg = pcx::scoped_push(c.containers, Sym::Type::Class);

        n->body = CommonParser::blockContents(c, false);
    }
    else
    {
        c.scanner.next(true);
    }
}

}

void Parser::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: buildNamespace(c, block, true); break;
        case Token::Type::RwFunc: buildFunc(c, block, true); break;
        case Token::Type::RwVar: buildVar(c, block, true); break;
        case Token::Type::RwClass: buildClass(c, block, true); break;

        default: throw Error(tok.location(), "parse error - ", tok.text());
    }
}

NodePtr Parser::build(Context &c)
{
    auto cg = pcx::scoped_push(c.containers, Sym::Type::Namespace);

    auto block = new BlockNode({ });
    NodePtr n(block);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block, false);
    }

    return n;
}
