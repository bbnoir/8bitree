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

Tree *Tree::modify(int times)
{
    Tree *newTree = new Tree(*this);
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
                if (newTree->treeAry[idx] >= 3 && idx + 1 < newTree->maxDim)
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
                if (newTree->treeAry[idx] >= 1 && newTree->treeAry[idx - 1] >= 2 && idx + 1 < newTree->maxDim)
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
        newTree->checkKraft();
    }
    return newTree;
}

void Tree::testModify(int times)
{
    Tree *tree = new Tree(256);
    cout << *tree << endl;
    while (times--)
    {

        int m = rand() % 6 + 1;
        Tree *newTree = tree->modify(m);
        delete tree;
        tree = newTree;
        cout << *tree << endl;
    }
}