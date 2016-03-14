#include <string>

using namespace std;

class NameTree {
public:
    NameTree(string name);

    void addChild(NameTree *child);
    void addData(string data);

    void display(int indents);

    NameTree *findNode(vector<string> target, int index);
    NameTree *findParent(vector<string> target, int index);
    vector<string> childList();
    vector<string> anonymousChildList();

    string name;
    string salt;
    string target;

    vector<string> dataEntries;
    vector<NameTree*> children;
};
