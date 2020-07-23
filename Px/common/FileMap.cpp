#include "FileMap.h"

FileMap::FileMap()
{
}

void FileMap::begin(char type, const std::string &name, pcx::optional<pcx::callback<std::size_t> > callback)
{
    v.push_back({ type, name, 0, { } });
    cb = callback;
}

void FileMap::setCurrentSize(std::size_t size)
{
    v.back().size = size;
}

void FileMap::write(std::ostream &os) const
{
    for(auto &e: v)
    {
        os << e.type << ", \"" << e.name << "\", " << e.size << "\n";
        os << "{\n";

        for(auto &c: e.comments)
        {
            os << "    " << c.address << ", \"" << c.comment << "\";\n";
        }

        os << "}\n";
    }
}
