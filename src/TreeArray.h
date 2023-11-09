#pragma once
#include "DataLoader.h"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

class TreeArray
{
private:
    vector<int> treeAry;
    int dim;
    set<int> leafSet;
    const int maxDim = 32;

    bool checkKraft();

public:
    TreeArray(int numLeaf);
    TreeArray *modify(int times = 1);
    friend ostream &operator<<(ostream &os, const TreeArray &tree);
    int getMinWidth(DataLoader *dl);
    static void testModify(int times = 1);
    TreeArray *getHuffman(DataLoader *dl);
};