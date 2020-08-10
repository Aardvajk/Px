#ifndef COMMONGENERATOR_H
#define COMMONGENERATOR_H

#include <iostream>

class Context;
class Node;

namespace CommonGenerator
{

void generateBooleanExpr(Context &c, std::ostream &os, Node *expr);

}

#endif // COMMONGENERATOR_H
