#include "CommonGenerator.h"

#include "common/Primitives.h"

#include "application/Context.h"

#include "generator/ExprGenerator.h"

#include "types/Type.h"
#include "types/TypeQuery.h"

void CommonGenerator::generateBooleanExpr(Context &c, std::ostream &os, Node *expr)
{
    ExprGenerator::generate(c, expr, os);
    os << "    convert " << Primitive::toString(TypeQuery::assert(c, expr)->primitiveType()) << " char;\n";
}
