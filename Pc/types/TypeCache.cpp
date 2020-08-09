#include "TypeCache.h"

TypeCache::TypeCache()
{
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

Type *TypeCache::nullType()
{
    return v.ptr(0);
}

Type *TypeCache::charType()
{
    return v.ptr(1);
}

Type *TypeCache::intType()
{
    return v.ptr(2);
}

Type *TypeCache::boolType()
{
    return v.ptr(3);
}
