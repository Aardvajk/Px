#ifndef OBJECT_H
#define OBJECT_H

#include <pcx/ptr_vector.h>
#include <pcx/datastream.h>

#include <string>
#include <vector>

namespace Object
{

class Link
{
public:
    Link(){ }
    Link(std::size_t address, std::size_t id) : address(address), id(id) { }

    std::size_t address;
    std::size_t id;
};

class Entity
{
public:
    char type;
    std::size_t id;
    std::size_t offset;
    std::vector<Link> links;
    std::vector<char> data;
};

class Unit
{
public:
    explicit Unit(std::string path) : path(std::move(path)) { }

    std::string path;
    std::vector<std::string> strings;
    pcx::ptr_vector<Entity> entities;
};

void load(pcx::data_istream &is, Entity &e);

}

#endif // OBJECT_H
