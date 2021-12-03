#ifndef NODE_H
#define NODE_H

#include "scanner/Location.h"

#include <pcx/shared_ptr.h>
#include <pcx/any.h>

#include <vector>
#include <string>
#include <unordered_map>

class Node;

using NodePtr = pcx::shared_ptr<Node>;
using NodeList = std::vector<pcx::shared_ptr<Node> >;

class Visitor;

class Node
{
public:
    explicit Node(Location location);
    virtual ~Node();

    NodePtr clone() const;

    void setProperty(const std::string &name, pcx::any value);

    virtual void accept(Visitor &v) = 0;
    virtual NodePtr cloneDetail() const = 0;

    std::string description() const;

    pcx::any property(const std::string &name) const;
    pcx::any assertedProperty(const std::string &name) const;

    Location location() const { return n; }

protected:
    static NodePtr safeClone(const NodePtr &n);
    static NodeList safeClone(const NodeList &n);

private:
    Location n;
    std::unordered_map<std::string, pcx::any> pm;
};

#endif // NODE_H
