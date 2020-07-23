#ifndef FILEMAP_H
#define FILEMAP_H

#include <pcx/optional.h>
#include <pcx/callback.h>
#include <pcx/str.h>

#include <vector>
#include <string>
#include <iostream>

class FileMap
{
public:
    struct Comment
    {
        std::size_t address;
        std::string comment;
    };

    struct Entry
    {
        char type;
        std::string name;
        std::size_t size;
        std::vector<Comment> comments;
    };

    FileMap();

    void begin(char type, const std::string &name, pcx::optional<pcx::callback<std::size_t> > callback);
    void setCurrentSize(std::size_t size);

    void push_back(const Entry &e){ v.push_back(e); }

    template<typename... Args> FileMap &operator()(Args&&... args);

    void read(std::istream &is);
    void write(std::ostream &os) const;

    Entry &operator[](std::size_t index){ return v[index]; }
    const Entry &operator[](std::size_t index) const { return v[index]; }

    std::size_t size() const { return v.size(); }

private:
    std::vector<Entry> v;
    pcx::optional<pcx::callback<std::size_t> > cb;
};

template<typename... Args> FileMap &FileMap::operator()(Args&&... args)
{
    v.back().comments.push_back({ cb ? (*cb)() : 0, pcx::str(std::forward<Args>(args)...) });
    return *this;
}

#endif // FILEMAP_H
