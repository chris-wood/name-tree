#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>

#include "name_tree.hpp"

void
disp(vector<string> data)
{
    for (vector<string>::iterator itr = data.begin(); itr != data.end(); itr++) {
        cout << *itr << "/";
    }
    cout << endl;
}

// https://www.safaribooksonline.com/library/view/c-cookbook/0596007612/ch04s07.html
vector<string>
split(const string& s, char c) {
    vector<string> split;
    string::size_type i = 0;
    string::size_type j = s.find(c);

    while (j != string::npos) {
        split.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);

        if (j == string::npos) {
            split.push_back(s.substr(i, s.length()));
        }
    }

    if (split.size() == 0) {
        split.push_back(s);
    }

    return split;
}

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

string
SHA256HashString(string aString) {
    string digest;
    CryptoPP::SHA256 hash;

    CryptoPP::StringSource foo(aString, true,
    new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

    return digest;
}

NameTree::NameTree(string new_name)
{
    name = new_name;

    // generate salt and compute the target
    salt = randomString(128);
    target = SHA256HashString(name + salt);
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

vector<string>
NameTree::anonymousChildList()
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

int
main()
{
    string rootName = "foo";
    NameTree *root = new NameTree(rootName);

    string rootData = "random";
    root->addData(rootData);

    string child1Name = "bar";
    NameTree *child1 = new NameTree(child1Name);
    root->addChild(child1);

    string child1Data = "baz";
    child1->addData(child1Data);

    string targetString = "foo/bar";
    vector<string> target = split(targetString, '/');
    // disp(target);
    // NameTree *found = root->findNode(target, 0);
    // cout << targetString << " found? " << (found != NULL) << endl;
    // if (found != NULL) {
    //     found->display(0);
    // }

    // targetString = "random";
    // target = split(targetString, '/');
    // disp(target);
    // found = root->findNode(target, 0);
    // cout << targetString << " found? " << (found != NULL) << endl;
    // if (found != NULL) {
    //     found->display(0);
    // }

    // targetString = "bar/baz";
    // target = split(targetString, '/');
    // found = root->findNode(target, 0);
    // cout << targetString << " found? " << (found != NULL) << endl;
    // vector<string> gap = root->findGap(target, 0);
    // for (vector<string>::iterator itr = gap.begin(); itr != gap.end(); itr++) {
    //     cout << *itr << ", ";
    // }
    // cout << endl;

    root->display(0);

    targetString = "foo/bar/bad";
    target = split(targetString, '/');
    NameTree *found = root->findNode(target, 0);
    cout << targetString << " found? " << (found != NULL) << endl;

    NameTree *parent = root->findParent(target, 0);
    parent->display(0);
    cout << "NAME " << parent->name << endl;


    vector<string> children = parent->childList();
    for (vector<string>::iterator itr = children.begin(); itr != children.end(); itr++) {
        cout << *itr << ", ";
    }
    cout << endl;
    // NameTree *randomRoot = generateRandomTree(10, 5);
    // randomRoot->display(0);
}
