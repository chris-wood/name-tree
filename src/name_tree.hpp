#include <string>
#include "cJSON.h"

class NameTree {
public:
    NameTree(std::string name);

    void addChild(NameTree *child);
    void addData(std::string data);

    void display(int indents);

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
