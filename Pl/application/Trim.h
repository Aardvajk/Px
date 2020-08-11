#ifndef TRIM_H
#define TRIM_H

class Context;
class FileMap;

namespace Object
{

class Entity;

}

namespace Trim
{

void generateRefs(Context &c, Object::Entity *e);
void updateFileMap(Context &c, FileMap &dm);

}

#endif // TRIM_H
