#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Tree
{
private:
    vector<int> treeAry;
    int dim;
    set<int> leafSet;
    const int maxDim = 32;

    bool checkKraft();

public:
    Tree(int numLeaf);
    Tree *modify(int times = 1);
    friend ostream &operator<<(ostream &os, const Tree &tree);
    static void testModify(int times = 1);
};
