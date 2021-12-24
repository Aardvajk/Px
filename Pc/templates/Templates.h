#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "nodes/Node.h"

class Context;
class Sym;
class IdNode;

namespace Templates
{

Sym *generateClass(Context &c, Sym *sym, NodeList &params);
Sym *generateFunc(Context &c, Sym *sym, IdNode &id);

}

#endif // TEMPLATES_H
