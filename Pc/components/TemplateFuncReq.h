#ifndef TEMPLATEFUNCREQ_H
#define TEMPLATEFUNCREQ_H

#include <unordered_map>

class TemplateFuncNode;
class Sym;
class Type;

class TemplateFuncReq
{
public:
    TemplateFuncReq(TemplateFuncNode *node, Sym *sym, const std::unordered_map<std::string, Type*> &map) : node(node), sym(sym), map(map) { }

    TemplateFuncNode *node;
    Sym *sym;
    std::unordered_map<std::string, Type*> map;
};

#endif // TEMPLATEFUNCREQ_H
