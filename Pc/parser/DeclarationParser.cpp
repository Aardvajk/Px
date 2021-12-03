#include "DeclarationParser.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"

namespace
{

void buildNamespace(Context &c, BlockNode *block, bool get)
{
    auto n = new NamespaceNode(c.scanner.token().location());
    block->push_back(n);

    n->name = CommonParser::name(c, get);

    n->body = CommonParser::blockContents(c, false);
}

void buildFunc(Context &c, BlockNode *block, bool get)
{
    auto n = new FuncNode(c.scanner.token().location());
    block->push_back(n);

    n->name = CommonParser::name(c, get);

    if(c.scanner.token().type() != Token::Type::Semicolon)
    {
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
