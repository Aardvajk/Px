#include "PrimitiveTypes.h"

#include "application/Context.h"

#include <pcx/scoped_ptr.h>

namespace
{

Sym *createPrimitive(Context &c, const std::string &name)
{
    pcx::scoped_ptr<Sym> s = new Sym(Sym::Type::Class, { }, name);

    s->setProperty("type", c.types.insert(Type::primary(s.get())));

    return s.release();
}

}

void PrimitiveTypes::create(Context &c)
{
    auto ns = c.tree.current()->add(new Sym(Sym::Type::Namespace, { }, "std"));

    ns->add(createPrimitive(c, "int"));
}
