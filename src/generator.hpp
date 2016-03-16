#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include "name_tree.hpp"

class Generator {
public:
    Generator();

    // TODO
    NameTree *generateRandomNode(int nameSize, int dataCount);
    NameTree *generateRandomTree(int maxChildren, int maxDepth);
};

#endif 
