#include <string>
#include <iostream>
#include <vector>

#include "name_tree.hpp"
#include "hasher.hpp"

using namespace std;

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

bool
targetInClearGap(vector<string> gap, string target)
{
    cout << "searching for " << target << endl;
    for (vector<string>::iterator itr = gap.begin(); itr != gap.end(); itr++) {
        cout << "against " << *itr << endl;
        if (target == *itr) {
            return true;
        }
    }
    return false;
}

// http://stackoverflow.com/questions/5612182/convert-string-with-explicit-escape-sequence-into-relative-character
string unescape(const string& s)
{
  string res;
  string::const_iterator it = s.begin();
  while (it != s.end())
  {
    char c = *it++;
    if (c == '\\' && it != s.end())
    {
      switch (*it++) {
      case '\\': c = '\\'; break;
      case 'n': c = '\n'; break;
      case 't': c = '\t'; break;
      // all other escapes
      default:
        // invalid escape sequence - skip it. alternatively you can copy it as is, throw an exception...
        continue;
      }
    }
    res += c;
  }

  return res;
}

bool
targetInAnonymousGap(cJSON *array, string input)
{
    Hasher hasher;

    int size = cJSON_GetArraySize(array);
    for (int i = 0; i < size; i++) {
        cJSON *item = cJSON_GetArrayItem(array, i);
        cJSON *saltJson = cJSON_GetObjectItem(item, "salt");
        cJSON *targetJson = cJSON_GetObjectItem(item, "target");

        char *rawSalt = cJSON_PrintUnformatted(saltJson);
        string salt = string(rawSalt);
        salt = unescape(string(salt, 1, salt.size() - 2));
        char *rawTarget = cJSON_PrintUnformatted(targetJson);
        string target = string(rawTarget);
        target = unescape(string(target, 1, target.size() - 2));

        string dataTarget = hasher.Hash(input + salt);

        cout << "input = " << input << endl;
        cout << "salt = " << salt << endl;
        cout << "target = " << dataTarget << endl;
        cout << "raw = " << target << endl;

        if (target == dataTarget) {
            return true;
        }
    }
    return false;
}

int
main()
{
    // Create the tree
    string rootName = "foo";
    NameTree *root = new NameTree(rootName);

    string rootData = "random";
    root->addData(rootData);

    string child1Name = "bar";
    NameTree *child1 = new NameTree(child1Name);
    root->addChild(child1);

    string child1Data = "baz";
    child1->addData(child1Data);

    // Display it
    root->display(0);

    // Do some hunting
    string targetString = "foo/bar/bad";
    vector<string> target = split(targetString, '/');
    NameTree *found = root->findNode(target, 0);
    cout << targetString << " found? " << (found != NULL) << endl;

    NameTree *parent = root->findParent(target, 0);
    cout << "NAME " << parent->name << endl;
    parent->display(0);

    vector<string> children = parent->childList();
    disp(children);

    targetString = "bad";
    bool inGap = targetInClearGap(children, targetString);
    cout << targetString << "In gap? " << inGap << endl;

    targetString = "baz";
    inGap = targetInClearGap(children, targetString);
    cout << targetString << "In gap? " << inGap << endl;

    cJSON *anonymousGap = parent->anonymousChildList();
    targetString = "baz";
    inGap = targetInAnonymousGap(anonymousGap, targetString);
    cout << targetString << "In gap? " << inGap << endl;
}
