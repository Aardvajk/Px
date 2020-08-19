#ifndef GENERICS_H
#define GENERICS_H

#include <string>
#include <vector>
#include <iostream>

class Context;
class Sym;
class Type;

namespace Generics
{

std::string funcname(const Sym *sym, const std::vector<Type*> &types);

void fulfilFuncs(Context &c, std::ostream &os);

}

#endif // GENERICS_H
