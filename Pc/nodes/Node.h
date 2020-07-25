#ifndef NODE_H
#define NODE_H

#include "scanner/Location.h"

#include <pcx/shared_ptr.h>

#include <vector>
#include <string>

class Node;

using NodePtr = pcx::shared_ptr<Node>;
using NodeList = std::vector<pcx::shared_ptr<Node> >;

class Visitor;

class Node
{
public:
    explicit Node(Location location);
    virtual ~Node();

    virtual void accept(Visitor &v) = 0;
    virtual std::string classname() const = 0;

    Location location() const { return n; }

private:
    Location n;
};

#endif // NODE_H
