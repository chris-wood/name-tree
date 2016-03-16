#ifndef NAME_TREE_HPP_
#define NAME_TREE_HPP_

#include <string>
#include <vector>
#include "cJSON.h"

class NameTree {
public:
    NameTree(std::string name);

    void addChild(NameTree *child);
    void addData(std::string data);

    void display(int indents);

    // TODO -> encode/decode the tree to/from JSON so pre-populate apps
    // - toJSON
    // - fromJSON

    NameTree *findNode(std::vector<std::string> target, int index);
    NameTree *findParent(std::vector<std::string> target, int index);

    std::vector<std::string> childList();
    cJSON *anonymousChildList();

    // Plaintext name
    std::string name;

    // Salted name
    std::string salt;
    std::string target;

    std::vector<std::string> dataEntries;
    std::vector<NameTree*> children;
};

#endif
