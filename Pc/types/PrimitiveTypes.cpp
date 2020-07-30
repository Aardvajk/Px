#include "PrimitiveTypes.h"

#include "application/Context.h"

#include <pcx/scoped_ptr.h>

namespace
{

Sym *createPrimitive(Context &c, const std::string &name, std::size_t size)
{
    pcx::scoped_ptr<Sym> s = new Sym(Sym::Type::Class, { }, name);

    s->setProperty("type", c.types.insert(Type::primary(s.get())));
    s->setProperty("size", size);

    return s.release();
}

}

void PrimitiveTypes::create(Context &c)
{
    auto ns = c.tree.current()->add(new Sym(Sym::Type::Namespace, { }, "std"));

    ns->add(createPrimitive(c, "null",  0));
    ns->add(createPrimitive(c, "char", 1));
    ns->add(createPrimitive(c, "int", 4));
}
