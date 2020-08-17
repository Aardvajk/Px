#ifndef GENERICS_H
#define GENERICS_H

#include <string>
#include <vector>
#include <iostream>

class Context;
class Sym;
class Type;
class Node;

namespace Generics
{

std::string funcname(const Sym *sym, const std::vector<Type*> &types);
std::string classname(const Sym *sym, const std::vector<Type*> &types);

Sym *fulfilType(Context &c, Sym *sym, const std::vector<Type*> &types);

void fulfilFuncs(Context &c, std::ostream &os);

bool anyGenerics(const std::vector<Type*> &types);

Type *updateTypeFromTarget(Context &c, Type *type, Node *target);

}

#endif // GENERICS_H
