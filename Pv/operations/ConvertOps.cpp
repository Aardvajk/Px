#include "ConvertOps.h"

#include "framework/ByteReader.h"

#include "common/Primitives.h"

#include "components/Stack.h"

namespace
{

template<typename From, typename To> void process(Stack &s)
{
    From v;

    s.pop(v);
    s.push(static_cast<To>(v));
}

}

void ConvertOps::convert(ByteReader &rm, Stack &s)
{
    using namespace Primitive;

    auto p0 = rm.get<Type>();
    auto p1 = rm.get<Type>();

    switch(p0)
    {
        case Type::Char:
        {
            switch(p1)
            {
                case Type::Int: process<char, int>(s); break;

                default: break;
            }
        }

        case Type::Int:
        {
            switch(p1)
            {
                case Type::Char: process<int, char>(s); break;

                default: break;
            }
        }

        default: break;
    }
}
