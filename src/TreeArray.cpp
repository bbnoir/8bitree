#include "TreeArray.h"
#include "CodeTree.h"
#include "Constants.h"
#include "FrequencyTable.h"
#include <cmath>
#include <cstdint>
#include <vector>

TreeArray::TreeArray(int numLeaf) : treeAry(ARRAY_SIZE, 0), prevAry(ARRAY_SIZE, 0)
{
    // balance tree
    if (numLeaf == SYM_NUM)
    {
        treeAry[8] = numLeaf;
        leafSet.insert(8);
    }
    else
    {
        treeAry[8] = numLeaf;
        int i = 8;
        int j = 7;
        while (!checkKraft())
        {
            treeAry[i] -= 1;
            treeAry[j] += 1;
            if (treeAry[i] == 0)
                leafSet.erase(i);
            if (treeAry[j] == 1)
                leafSet.insert(j);
            if (treeAry[i] == 0)
            {
                i--;
                j--;
            }
            if (treeAry[j] == pow(2, j))
            {
                j--;
            }
            if (j == -1)
            {
                cout << "Error: Cannot balance tree" << endl;
                exit(1);
            }
        }
    }
}

void buildCodeLengths(const Node *node, uint32_t depth, vector<uint32_t>& codeLengths) {
    if (dynamic_cast<const InternalNode*>(node) != nullptr) {
        const InternalNode *internalNode = dynamic_cast<const InternalNode*>(node);
        buildCodeLengths(internalNode->leftChild .get(), depth + 1, codeLengths);
        buildCodeLengths(internalNode->rightChild.get(), depth + 1, codeLengths);
    } else if (dynamic_cast<const Leaf*>(node) != nullptr) {
        uint32_t symbol = dynamic_cast<const Leaf*>(node)->symbol;
        if (symbol >= codeLengths.size())
            throw std::invalid_argument("Symbol exceeds symbol limit");
        // Note: CodeTree already has a checked constraint that disallows a symbol in multiple leaves
        if (codeLengths.at(symbol) != 0)
            throw std::logic_error("Assertion error: Symbol has more than one code");
        codeLengths.at(symbol) = depth;
    } else {
        throw std::logic_error("Assertion error: Illegal node type");
    }
}

vector<short> CodeLen2TreeAry(vector<uint32_t> cl)
{
    vector<short> tree(ARRAY_SIZE, 0);
    for (int i = 0; i < SYM_NUM; i++)
        tree[cl[i]]++;
    tree[0] = 0;
    return tree;
}

TreeArray::TreeArray(vector<freq_t> freq) : treeAry(ARRAY_SIZE, 0), prevAry(ARRAY_SIZE, 0)
{
    // Huffman
    FrequencyTable freqs(freq);
    CodeTree codeTree = freqs.buildCodeTree();
    vector<uint32_t> codeLengths(SYM_NUM, 0);
    buildCodeLengths(&codeTree.root, 0, codeLengths);
    treeAry = CodeLen2TreeAry(codeLengths);
    if (!checkKraft())
    {
        cout << "Kraft!" << endl;
        exit(1);
    }
    for (int i = 0; i < ARRAY_SIZE; i++)
        if (treeAry[i] != 0)
            leafSet.insert(i);
}

TreeArray::TreeArray(const TreeArray &tree) : treeAry(tree.treeAry), prevAry(tree.prevAry), leafSet(tree.leafSet), prevLeafSet(tree.prevLeafSet) {}

vector<short> TreeArray::getCodeArray()
{
    return treeAry;
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
                    mod_success = checkKraft();
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
                    mod_success = checkKraft();
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
                    mod_success = checkKraft();
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
                    mod_success = checkKraft();
                }
                break;
            default:
                cout << "Error: Invalid mode" << endl;
                break;
            }
        }
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

int TreeArray::getMaxLen()
{
    int maxLen = ARRAY_SIZE - 1;
    while (treeAry[maxLen] == 0)
        maxLen--;
    return maxLen;
}

int TreeArray::getMinLen()
{
    int minLen = 0;
    while (treeAry[minLen] == 0)
        minLen++;
    return minLen;
}