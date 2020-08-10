#include "CommonGenerator.h"

#include "common/Primitives.h"

#include "application/Context.h"

#include "generator/ExprGenerator.h"

#include "types/Type.h"
#include "types/TypeQuery.h"

void CommonGenerator::generateBooleanExpr(Context &c, std::ostream &os, Node *expr)
{
    auto type = TypeQuery::assert(c, expr);

    ExprGenerator::generate(c, expr, os);
    if(!Type::exact(type, c.types.boolType()) && !Type::exact(type, c.types.charType()))
    {
        os << "    convert " << Primitive::toString(type->primitiveType()) << " char;\n";
    }
}
