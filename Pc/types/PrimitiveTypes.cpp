#include "PrimitiveTypes.h"

#include "common/Primitives.h"

#include "application/Context.h"

#include <pcx/scoped_ptr.h>

namespace
{

Sym *createPrimitive(Context &c, const std::string &name, Primitive::Type type, std::size_t size)
{
    pcx::scoped_ptr<Sym> s = new Sym(Sym::Type::Class, { }, name);

    s->setProperty("type", c.types.insert(Type::primary(s.get())));
    s->setProperty("primitive", type);
    s->setProperty("size", size);

    return s.release();
}

}

void PrimitiveTypes::create(Context &c)
{
    auto ns = c.tree.current()->add(new Sym(Sym::Type::Namespace, { }, "std"));

    ns->add(createPrimitive(c, "null", Primitive::Type::Null,  0));
    ns->add(createPrimitive(c, "char", Primitive::Type::Char, 1));
    ns->add(createPrimitive(c, "int", Primitive::Type::Int, 4));
    ns->add(createPrimitive(c, "bool", Primitive::Type::Char, 1));
    ns->add(createPrimitive(c, "ulong", Primitive::Type::ULong, 8));
}
