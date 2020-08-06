#ifndef DIRECTIVEPARSER_H
#define DIRECTIVEPARSER_H

class Context;
class BlockNode;

namespace DirectiveParser
{

void buildInclude(Context &c, BlockNode *block, bool get);

}

#endif // DIRECTIVEPARSER_H
