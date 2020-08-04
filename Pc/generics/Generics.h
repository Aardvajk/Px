#ifndef GENERICS_H
#define GENERICS_H

#include <string>
#include <vector>

class Sym;
class Type;

namespace Generics
{

std::string funcname(const Sym *sym, const std::vector<Type*> &types);

}

#endif // GENERICS_H
