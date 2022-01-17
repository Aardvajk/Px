#include "TypeCache.h"

#include "application/Context.h"

namespace
{

Sym *createUnknownTemplateType(Context &c)
{
    pcx::scoped_ptr<Sym> s = new Sym(Sym::Type::Class, { }, "#unknowntemplate");

    s->setProperty("type", c.types.insert(Type::primary(s.get())));

    return s.release();
}

}

TypeCache::TypeCache(Context &c)
{
    auto ns = c.tree.current()->add(new Sym(Sym::Type::Namespace, { }, "std"));

    createPrimitive(c, "null", ns, Primitive::Type::Null, 0);
    createPrimitive(c, "char", ns, Primitive::Type::Char, 1);
    createPrimitive(c, "int", ns, Primitive::Type::Int, 4);

    unknownTemplateSym = createUnknownTemplateType(c);
}

Type *TypeCache::insert(const Type &type)
{
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        if(Type::exact(&type, v.ptr(i)))
        {
            return v.ptr(i);
        }
    }

    v.push_back(new Type(type));
    return v.back_ptr();
}

Type *TypeCache::primitiveType(Primitive::Type type)
{
    return v.ptr(tm[static_cast<std::underlying_type<Primitive::Type>::type>(type)]);
}

bool TypeCache::isUnknownTemplateType(const Type *type) const
{
    auto t = *type;
    t.ref = false;

    return Type::compare(&t, unknownTemplateSym->property("type").to<Type*>());
}

Type *TypeCache::unknownTemplateType()
{
    return unknownTemplateSym->property("type").to<Type*>();
}

void TypeCache::createPrimitive(Context &c, const std::string &name, Sym *container, Primitive::Type type, std::size_t size)
{
    tm[static_cast<std::underlying_type<Primitive::Type>::type>(type)] = container->children().size();

    auto s = new Sym(Sym::Type::Class, { }, name);
    container->add(s);

    s->setProperty("type", c.types.insert(Type::primary(s)));
    s->setProperty("primitive", type);
    s->setProperty("size", size);
}
