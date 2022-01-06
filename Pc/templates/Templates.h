#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "nodes/Node.h"

class Context;
class Sym;
class Type;
class IdNode;

namespace Templates
{

Sym *generateClass(Context &c, Sym *sym, NodeList &params);
Sym *generateFuncReq(Context &c, Sym *sym, Type *expected, IdNode &id);

void fullfillFuncReqs(Context &c);

}

#endif // TEMPLATES_H
