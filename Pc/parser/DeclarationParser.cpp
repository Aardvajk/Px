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

void buildFunc(Context &c, BlockNode *block, bool get)
{
    auto n = new FuncNode(c.scanner.token().location());
    block->push_back(n);

    n->name = CommonParser::name(c, get);

    if(c.containers.back() != Sym::Type::Namespace)
    {
        throw Error(n->name->location(), "invalid location for function - ", n->name->description());
    }

    c.scanner.match(Token::Type::LeftParen, false);
    c.scanner.consume(Token::Type::RightParen, true);

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

}

void DeclarationParser::build(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: buildNamespace(c, block, true); break;
        case Token::Type::RwFunc: buildFunc(c, block, true); break;

        default: break;
    }
}
