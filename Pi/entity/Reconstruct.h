#ifndef RECONSTRUCT_H
#define RECONSTRUCT_H

#include <iostream>

class Context;
class Entity;

namespace Reconstruct
{

void toStream(Context &c, Entity &root, std::ostream &os);

}

#endif // RECONSTRUCT_H
