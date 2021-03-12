#ifndef OBJECT_H
#define OBJECT_H

#include <pcx/ptr_vector.h>
#include <pcx/datastream.h>

#include <vector>
#include <string>
#include <cstdint>

namespace Object
{

class Entity
{
public:
    char type;
    std::uint32_t flags;
    std::size_t id;

    std::size_t offset;
    std::vector<char> data;
};

class File
{
public:
    File(const std::string &path) : path(path) { }

    std::string path;

    std::vector<std::string> strings;
    pcx::ptr_vector<Entity> entities;
};

void load(pcx::data_istream &is, Entity &e);

}

#endif // OBJECT_H
