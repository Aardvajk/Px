#include "Generics.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

std::string Generics::funcname(const Sym *sym, const std::vector<Type*> &types)
{
    return pcx::str(sym->fullname(), "<", pcx::join_str(types, ",", [](const Type *t){ return t->description(); }), ">", sym->assertProperty("type").to<Type*>()->convertedDescription(types));
}
