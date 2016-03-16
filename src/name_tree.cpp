#include <string>
#include <vector>
#include <iostream>

#include "name_tree.hpp"
#include "hasher.hpp"

using namespace std;

// http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
void
gen_random(char *s, const int len)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

string
randomString(size_t length)
{
    string str(length, 0);
    gen_random((char *) str.c_str(), length);
    return str;
}

NameTree::NameTree(string new_name)
{
    name = new_name;

    // generate salt and compute the target
    salt = randomString(128);

    Hasher hasher;
    target = hasher.Hash(name + salt);
}

void
NameTree::addChild(NameTree *child)
{
    children.push_back(child);
}

void
NameTree::addData(string data)
{
    dataEntries.push_back(data);
}

// foo, bar, baz
// foo, bar
// foo
NameTree *
NameTree::findNode(vector<string> target, int index) {
    if (index == target.size() - 1) {
        string targetData = target[index];
        for (vector<string>::iterator itr = dataEntries.begin(); itr != dataEntries.end(); itr++) {
            if (targetData == *itr) {
                return this;
            }
        }
        return NULL;
    } else {
        string childName = target[index];

        if (childName != name) {
            return NULL;
        }

        for (vector<NameTree*>::iterator itr = children.begin(); itr != children.end(); itr++) {
            NameTree *found = (*itr)->findNode(target, index + 1);
            if (found != NULL) {
                return found;
            }
        }
        return NULL;
    }
}

NameTree *
NameTree::findParent(vector<string> target, int index) {
    if (index == target.size() - 1) {
        return this;
    } else {
        NameTree *child = NULL;
        for (vector<NameTree*>::iterator itr = children.begin(); itr != children.end(); itr++) {
            NameTree *found = (*itr)->findParent(target, index + 1);
            if (found != NULL) {
                child = found;
                break;
            }
        }

        child = child == NULL ? this : child;
        return child;
    }
}

vector<string>
NameTree::childList()
{
    vector<string> childNames;
    for (vector<NameTree*>::iterator itr = children.begin(); itr != children.end(); itr++) {
        childNames.push_back((*itr)->name);
    }
    for (vector<string>::iterator itr = dataEntries.begin(); itr != dataEntries.end(); itr++) {
        childNames.push_back(*itr);
    }
    return childNames;
}

cJSON *
NameTree::anonymousChildList()
{
    cJSON *array = cJSON_CreateArray();
    for (vector<NameTree*>::iterator itr = children.begin(); itr != children.end(); itr++) {
        cJSON *saltJson = cJSON_CreateString(salt.c_str());
        cJSON *targetJson = cJSON_CreateString(target.c_str());

        cJSON *record = cJSON_CreateObject();
        cJSON_AddItemToObject(record, "salt", saltJson);
        cJSON_AddItemToObject(record, "target", targetJson);

        cJSON_AddItemToArray(array, record);
    }
    for (vector<string>::iterator itr = dataEntries.begin(); itr != dataEntries.end(); itr++) {
        string dataSalt = randomString(128);

        Hasher hasher;

        string dataTarget = hasher.Hash(*itr + dataSalt);

        cout << "input = " << *itr << endl;
        cout << "salt = " << dataSalt << endl;
        cout << "target = " << dataTarget << endl;

        cJSON *saltJson = cJSON_CreateString(dataSalt.c_str());
        cJSON *targetJson = cJSON_CreateString(dataTarget.c_str());

        cJSON *record = cJSON_CreateObject();
        cJSON_AddItemToObject(record, "salt", saltJson);
        cJSON_AddItemToObject(record, "target", targetJson);

        cJSON_AddItemToArray(array, record);
    }
    return array;
}

NameTree *
generateRandomNode()
{
    string name = randomString(10);
    return new NameTree(name);
}

NameTree *
generateRandomTree(int maxChildren, int maxDepth)
{
    if (maxDepth == 0) {
        return NULL;
    }

    NameTree *node = generateRandomNode();

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

void
NameTree::display(int indent)
{
    for (int i = 0; i < indent; i++) {
        cout << "  ";
    }
    cout << "* " << name << endl;
    for (vector<NameTree*>::iterator itr = children.begin(); itr != children.end(); itr++) {
        (*itr)->display(indent + 1);
    }
    for (vector<string>::iterator itr = dataEntries.begin(); itr != dataEntries.end(); itr++) {
        for (int i = 0; i < indent + 1; i++) {
            cout << "  ";
        }
        cout << "- " << *itr << endl;
    }
}
