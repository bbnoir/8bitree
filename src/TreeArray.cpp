#include "TreeArray.h"
#include "Constants.h"
#include <cmath>

TreeArray::TreeArray(int numLeaf) : treeAry(ARRAY_SIZE, 0), prevAry(ARRAY_SIZE, 0)
{
    // balance tree
    treeAry[8] = numLeaf;
    leafSet.insert(8);
}

ostream &operator<<(ostream &os, const TreeArray &tree)
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        os << tree.treeAry[i] << " ";
    }
    return os;
}

bool TreeArray::checkKraft()
{
    double sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++)
        sum += treeAry[i] * pow(2, -i);
    if (sum != 1)
    {
        cerr << "Error: Kraft's inequality is not satisfied" << endl;
        cerr << "TreeArray:" << *this << endl;
        exit(1);
    }
    return sum == 1;
}

template <typename T>
T randIdx(set<T> const &s)
{
    auto it = s.cbegin();
    int random = rand() % s.size();
    advance(it, random);
    return *it;
}

void TreeArray::modify(int times = 1)
{
    // save current state for recovery
    prevAry = treeAry;
    prevLeafSet = leafSet;
    // modify for #times
    while (times--)
    {
        bool mod_success = false;
        while (!mod_success)
        {
            int mode = rand() % 4;
            int idx = randIdx(leafSet);
            switch (mode)
            {
            case 0: // [+1, -3, +2]
                if (idx > 0 && treeAry[idx] >= 3 && idx + 1 < ARRAY_SIZE)
                {
                    treeAry[idx] -= 3;
                    treeAry[idx + 1] += 2;
                    treeAry[idx - 1] += 1;
                    if (treeAry[idx] == 0)
                        leafSet.erase(idx);
                    leafSet.insert(idx - 1);
                    leafSet.insert(idx + 1);
                    mod_success = true;
                }
                break;
            case 1: // [-1, +3, -2]
                if (idx > 0 && treeAry[idx - 1] >= 1 && treeAry[idx + 1] >= 2 && idx + 1 < ARRAY_SIZE)
                {
                    treeAry[idx + 1] -= 2;
                    treeAry[idx] += 3;
                    treeAry[idx - 1] -= 1;
                    if (treeAry[idx + 1] == 0)
                        leafSet.erase(idx);
                    if (treeAry[idx - 1] == 0)
                        leafSet.erase(idx - 1);
                    leafSet.insert(idx);
                    mod_success = true;
                }
                break;
            case 2: // [+1, -2, -1, +2]
                if (idx > 1 && treeAry[idx] >= 1 && treeAry[idx - 1] >= 2 && idx + 1 < ARRAY_SIZE)
                {
                    treeAry[idx + 1] += 2;
                    treeAry[idx] -= 1;
                    treeAry[idx - 1] -= 2;
                    treeAry[idx - 2] += 1;
                    if (treeAry[idx] == 0)
                        leafSet.erase(idx);
                    if (treeAry[idx - 1] == 0)
                        leafSet.erase(idx - 1);
                    leafSet.insert(idx + 1);
                    leafSet.insert(idx - 2);
                    mod_success = true;
                }
                break;
            case 3: // [-1, +2, +1, -2]
                if (idx > 1 && treeAry[idx - 2] >= 1 && treeAry[idx + 1] >= 2 && idx + 1 < ARRAY_SIZE)
                {
                    treeAry[idx + 1] -= 2;
                    treeAry[idx] += 1;
                    treeAry[idx - 1] += 2;
                    treeAry[idx - 2] -= 1;
                    if (treeAry[idx + 1] == 0)
                        leafSet.erase(idx);
                    if (treeAry[idx - 2] == 0)
                        leafSet.erase(idx - 2);
                    leafSet.insert(idx);
                    leafSet.insert(idx - 1);
                    mod_success = true;
                }
                break;
            default:
                cout << "Error: Invalid mode" << endl;
                break;
            }
        }
        checkKraft();
    }
    return;
}

void TreeArray::recover()
{
    treeAry = prevAry;
    leafSet = prevLeafSet;
}

void TreeArray::testModify(int times = 1)
{
    TreeArray *tree = new TreeArray(256);
    cout << "iter: " << 0 << " tree: " << *tree << endl;
    for (int i = 1; i <= times; i++)
    {

        // int m = rand() % 6 + 1;
        int m = 1;
        tree->modify(m);
        if (i % 1000 == 0)
            cout << "iter: " << i << " tree: " << *tree << endl;
    }
}

int TreeArray::getMinWidth(DataLoader *dl)
{
    int minWidth = 0;
    // for (int i = 0; i < dl->getNumLines(); i++)
    // {
    //     ;
    // }
    minWidth = (rand() % 200) / 100.0 * dl->getNumElements() * 8;
    return minWidth;
}

TreeArray *TreeArray::genHuffmanArray(DataLoader *DL)
{
    TreeArray *tree = new TreeArray(DL->getNumInts());
    return tree;
}