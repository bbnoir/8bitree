#pragma once
#include "DataLoader.h"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

enum class initMode
{
    BALANCE,
    RANDOM,
    HUFFMAN,
    HALF
};

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
    TreeArray(const TreeArray &tree);
    vector<short> getCodeArray();
    friend ostream &operator<<(ostream &os, const TreeArray &tree);

    // for modify
    void modify(int times);
    void recover();
    static void testModify(int times);
    static TreeArray *genHuffmanArray(DataLoader *DL);
};