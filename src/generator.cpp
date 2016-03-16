#include <string>
#include <iostream>

#include "name_tree.hpp"
#include "generator.hpp"
#include "random.hpp"

using namespace std;

Generator::Generator()
{

}

NameTree *
Generator::generateRandomNode(int nameSize, int dataCount)
{
    string name = randomString(nameSize);
    NameTree *nameTree = new NameTree(name);

    for (int i = 0; i < dataCount; i++) {
        string data = randomString(nameSize);
        nameTree->addData(data);
    }

    return nameTree;
}

NameTree *
Generator::generateRandomTree(int maxChildren, int maxDepth)
{
    if (maxDepth == 0) {
        return NULL;
    }

    NameTree *node = generateRandomNode(10, 2);

    int numChildren = rand() % maxChildren + 1;
    if (numChildren > 0) {
        for (int i = 0; i < numChildren; i++) {
            NameTree *child = generateRandomTree(maxChildren, maxDepth - 1);
            if (child != NULL) {
                node->addChild(child);
            }
        }
    }

    return node;
}
