#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Tree
{
private:
    vector<int> treeAry;
    int dim;
    const int maxDim = 32;
    set<int> leafSet;
    void modify(int times = 1);
    bool checkKraft();
    bool split(const int &idx);

public:
    Tree(int numLeaf);
    friend ostream &operator<<(ostream &os, const Tree &tree);
    void testModify(int n);
};
