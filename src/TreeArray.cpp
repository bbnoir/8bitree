#include "TreeArray.h"
#include "Constants.h"
#include <cmath>

TreeArray::TreeArray(int numLeaf) : treeAry(ARRAY_SIZE, 0)
{
    dim = 16;
    treeAry[8] = numLeaf;
    leafSet.insert(8);
}

ostream &operator<<(ostream &os, const TreeArray &tree)
{
    for (int i = 0; i < tree.dim; i++)
    {
        os << tree.treeAry[i] << " ";
    }
    return os;
}

bool TreeArray::checkKraft()
{
    double sum = 0;
    for (int i = 0; i < dim; i++)
        sum += treeAry[i] * pow(2, -i);
    if (sum != 1)
    {
        cout << "Error: Check Kraft's sum: " << sum << " != 1" << endl;
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

TreeArray *TreeArray::modify(int times)
{
    TreeArray *newTree = new TreeArray(*this);
    while (times--)
    {
        bool mod_success = false;
        while (!mod_success)
        {
            int mode = rand() % 2;
            int idx = randIdx(newTree->leafSet);
            switch (mode)
            {
            case 0: // [+1, -3, +2]
                if (newTree->treeAry[idx] >= 3 && idx + 1 < ARRAY_SIZE)
                {
                    if (idx == newTree->dim - 1)
                    {
                        newTree->dim += 1;
                        newTree->treeAry.resize(newTree->dim + 1);
                    }
                    newTree->treeAry[idx] -= 3;
                    newTree->treeAry[idx + 1] += 2;
                    newTree->treeAry[idx - 1] += 1;
                    if (newTree->treeAry[idx] == 0)
                        newTree->leafSet.erase(idx);
                    newTree->leafSet.insert(idx - 1);
                    newTree->leafSet.insert(idx + 1);
                    mod_success = true;
                }
                break;
            case 1: // [+1, -2, -1, +2]
                if (newTree->treeAry[idx] >= 1 && newTree->treeAry[idx - 1] >= 2 && idx + 1 < ARRAY_SIZE)
                {
                    if (idx == newTree->dim - 1)
                    {
                        newTree->dim += 1;
                        newTree->treeAry.resize(newTree->dim + 1);
                    }
                    newTree->treeAry[idx + 1] += 2;
                    newTree->treeAry[idx] -= 1;
                    newTree->treeAry[idx - 1] -= 2;
                    newTree->treeAry[idx - 2] += 1;
                    if (newTree->treeAry[idx] == 0)
                        newTree->leafSet.erase(idx);
                    if (newTree->treeAry[idx - 1] == 0)
                        newTree->leafSet.erase(idx - 1);
                    newTree->leafSet.insert(idx + 1);
                    newTree->leafSet.insert(idx - 2);
                    mod_success = true;
                }
                break;
            default:
                cout << "Error: Invalid mode" << endl;
                break;
            }
        }
        // newTree->checkKraft();
    }
    return newTree;
}

void TreeArray::testModify(int times)
{
    TreeArray *tree = new TreeArray(256);
    cout << "iter: " << 0 << " tree: " << *tree << endl;
    for (int i = 1; i <= times; i++)
    {

        // int m = rand() % 6 + 1;
        int m = 1;
        TreeArray *newTree = tree->modify(m);
        delete tree;
        tree = newTree;
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