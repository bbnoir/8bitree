#pragma once
#include "DataLoader.h"
#include "Constants.h"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

class TreeArray
{
private:
    vector<short> treeAry;
    vector<short> prevAry;
    set<short> leafSet;
    set<short> prevLeafSet;

    bool checkKraft();

public:
    TreeArray(int numLeaf);
    TreeArray(vector<freq_t> freq);
    TreeArray(const TreeArray &tree);
    vector<short> getCodeArray();
    friend ostream &operator<<(ostream &os, const TreeArray &tree);

    int getMaxLen();
    int getMinLen();

    // for modify
    void modify(int times);
    void recover();
    static void testModify(int times);
};
