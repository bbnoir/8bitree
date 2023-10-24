#include "tree.h"
#include <cmath>

Tree::Tree(int numLeaf) : treeAry(16, 0)
{
    dim = 16;
    treeAry[8] = numLeaf;
    leafSet.insert(8);
}

ostream &operator<<(ostream &os, const Tree &tree)
{
    for (int i = 0; i < tree.dim; i++)
    {
        os << tree.treeAry[i] << " ";
    }
    return os;
}

bool Tree::checkKraft()
{
    double sum = 0;
    for (int i = 0; i < dim; i++)
        sum += treeAry[i] * pow(2, -i);
    if (sum != 1)
    {
        cout << "Error: Check Kraft's sum: " << sum << " != 1" << endl;
    }
    return sum == 1;
}

bool Tree::split(const int &idx)
{
    short mode = rand() % 2;
    switch (mode)
    {
    case 0:
        if (treeAry[idx] < 2)
            return false;
        treeAry[idx] -= 2;
        treeAry[idx - 1] += 1;
        if (treeAry[idx] == 0)
            leafSet.erase(idx);
        leafSet.insert(idx - 1);
        break;
    case 1:
        if (treeAry[idx] < 1 || idx + 2 >= maxDim)
            return false;
        if (idx == dim - 1)
        {
            dim += 2;
            treeAry.resize(dim + 2);
        }
        treeAry[idx] -= 1;
        treeAry[idx + 1] += 2;
        if (treeAry[idx] == 0)
            leafSet.erase(idx);
        leafSet.insert(idx + 1);
        break;
    default:
        break;
    }
    // if (!checkKraft())
    // {
    //     cout << "treeAry: " << *this << endl;
    //     cout << "Kraft inequality violated" << endl;
    //     exit(1);
    // }
    return true;
}

template <typename T>
T randIdx(set<T> const &s)
{
    auto it = s.cbegin();
    int random = rand() % s.size();
    advance(it, random);
    return *it;
}

void Tree::modify(int times)
{
    vector<int> oriAry = treeAry;
    int idx;
    while (times--)
        while (!split(randIdx(leafSet)))
            ;
}

void Tree::testModify(int n)
{
    while (n--)
    {
        modify(1);
        cout << *this << endl;
    }
}