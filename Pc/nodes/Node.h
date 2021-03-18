#ifndef NODE_H
#define NODE_H

#include "framework/Location.h"

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
    Node(Location location);
    virtual ~Node();

    NodePtr clone() const;

    virtual void accept(Visitor &v) = 0;
    virtual NodePtr cloneDetail() const = 0;

    std::string description() const;

    Location location() const { return n; }

protected:
    static NodePtr safeClone(const NodePtr &n);
    static NodeList safeClone(const NodeList &n);

private:
    Location n;
};

#endif // NODE_H
