#include "TypeCache.h"

#include "common/Primitives.h"

#include "application/Context.h"

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

TypeCache::TypeCache(Context &c)
{
    auto ns = c.tree.current()->add(new Sym(Sym::Type::Namespace, { }, "std"));

    ns->add(createPrimitive(c, "null", Primitive::Type::Null, 0));
    ns->add(createPrimitive(c, "char", Primitive::Type::Char, 1));
    ns->add(createPrimitive(c, "int", Primitive::Type::Int, 4));
}

Type *TypeCache::insert(const Type &type)
{
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        if(Type::compare(&type, v.ptr(i)))
        {
            return v.ptr(i);
        }
    }

    v.push_back(new Type(type));
    return v.back_ptr();
}

Type *TypeCache::nullType()
{
    return v.ptr(0);
}

Type *TypeCache::intType()
{
    return v.ptr(1);
}

void TypeCache::print(std::ostream &os)
{
    for(auto t: v)
    {
        os << t.description() << "\n";
    }
}
