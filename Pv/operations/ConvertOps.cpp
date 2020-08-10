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
                case Type::Int: process<char, int>(s); return;
                case Type::ULong: process<char, std::size_t>(s); return;

                default: return;
            }
        }

        case Type::Int:
        {
            switch(p1)
            {
                case Type::Char: process<int, char>(s); return;
                case Type::ULong: process<int, std::size_t>(s); return;

                default: return;
            }
        }

        case Type::ULong:
        {
            switch(p1)
            {
                case Type::Char: process<std::size_t, char>(s); return;
                case Type::Int: process<std::size_t, int>(s); return;

                default: return;
            }
        }

        default: return;
    }
}
