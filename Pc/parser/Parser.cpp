#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/Nodes.h"

#include "parser/CommonParser.h"
#include "parser/IncludeParser.h"
#include "parser/TypeParser.h"
#include "parser/FuncParser.h"
#include "parser/ExprParser.h"

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

void buildTemplateParameter(Context &c, NodeList &container, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);
    container.push_back(new IdNode(tok.location(), { }, tok.text()));

    c.scanner.next(true);
    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildTemplateParameter(c, container, true);
    }
}

void buildArgs(Context &c, NodeList &container, bool get)
{
    auto n = new VarNode(c.scanner.token().location());
    container.push_back(n);

    n->setProperty("arg", true);

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

template<typename T> void buildFuncImp(Context &c, T *n, bool get)
{
    n->name = CommonParser::name(c, get);

    if(c.containers.back() != Sym::Type::Namespace && c.containers.back() != Sym::Type::Class)
    {
        throw Error(n->name->location(), "invalid location for function - ", n->name->description());
    }

    if(c.scanner.token().type() == Token::Type::LeftParen)
    {
        c.scanner.consume(Token::Type::LeftParen, false);
        if(c.scanner.token().type() != Token::Type::RightParen)
        {
            buildArgs(c, n->args, false);
        }

        c.scanner.consume(Token::Type::RightParen, false);
    }

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

void buildFunc(Context &c, BlockNode *block, bool get)
{
    auto n = new FuncNode(c.scanner.token().location());
    block->push_back(n);

    buildFuncImp(c, n, get);
}

void buildTemplateFunc(Context &c, BlockNode *block, bool get)
{
    auto n = new TemplateFuncNode(c.scanner.token().location());
    block->push_back(n);

    c.scanner.consume(Token::Type::Lt, get);
    if(c.scanner.token().type() != Token::Type::Gt)
    {
        buildTemplateParameter(c, n->params, false);
    }

    c.scanner.match(Token::Type::Gt, false);

    buildFuncImp(c, n, true);
}

void buildFuncOrTemplate(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::Lt)
    {
        buildTemplateFunc(c, block, false);
    }
    else
    {
        buildFunc(c, block, false);
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

    if(c.scanner.token().type() == Token::Type::Assign)
    {
        n->value = ExprParser::build(c, true);
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

template<typename T> void buildClassImp(Context &c, T *n, bool get)
{
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

void buildClass(Context &c, BlockNode *block, bool get)
{
    auto n = new ClassNode(c.scanner.token().location());
    block->push_back(n);

    buildClassImp(c, n, get);
}

void buildTemplateClass(Context &c, BlockNode *block, bool get)
{
    auto n = new TemplateClassNode(c.scanner.token().location());
    block->push_back(n);

    c.scanner.consume(Token::Type::Lt, get);
    if(c.scanner.token().type() != Token::Type::Gt)
    {
        buildTemplateParameter(c, n->params, false);
    }

    c.scanner.match(Token::Type::Gt, false);

    buildClassImp(c, n, true);
}

void buildClassOrTemplate(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::Lt)
    {
        buildTemplateClass(c, block, false);
    }
    else
    {
        buildClass(c, block, false);
    }
}

}

void Parser::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwInclude: IncludeParser::build(c, block, true); break;

        case Token::Type::RwNamespace: buildNamespace(c, block, true); break;
        case Token::Type::RwFunc: buildFuncOrTemplate(c, block, true); break;
        case Token::Type::RwVar: buildVar(c, block, true); break;
        case Token::Type::RwClass: buildClassOrTemplate(c, block, true); break;

        default: FuncParser::build(c, block, false);
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
