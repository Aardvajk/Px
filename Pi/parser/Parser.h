#ifndef PARSER_H
#define PARSER_H

#include "entity/Entity.h"

class Context;

namespace Parser
{

EntityPtr build(Context &c);

}

#endif // PARSER_H
