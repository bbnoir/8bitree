#include "Encoder.h"
#include "Constants.h"
#include <algorithm>
#include <iostream>
#include <bitset>

using namespace std;

bool cmp(const pair<int, int> &a,
         const pair<int, int> &b)
{
    if (a.second == b.second)
        return a.first < b.first;
    return a.second < b.second;
}

// Function to sort the map according
// to value in a (key-value) pairs
vector<pair<int, int>> sort(vector<int> M)
{
    vector<pair<int, int>> A;
    for (int i = 0; i < M.size(); i++)
        A.push_back(make_pair(i + SYM_MIN, M[i]));
    sort(A.begin(), A.end(), cmp);
    return A;
}

bool checkTreeValid(vector<int> codeLengths)
{
    std::sort(codeLengths.begin(), codeLengths.end(), std::greater<int>());
    int currentLevel = codeLengths.front();
    int numNodesAtLevel = 0;
    for (int cl : codeLengths)
    {
        if (cl == 0)
            break;
        while (cl < currentLevel)
        {
            if (numNodesAtLevel % 2 != 0)
                throw std::invalid_argument("Under-full Huffman code tree");
            numNodesAtLevel /= 2;
            currentLevel--;
        }
        numNodesAtLevel++;
    }
    while (currentLevel > 0)
    {
        if (numNodesAtLevel % 2 != 0)
            throw std::invalid_argument("Under-full Huffman code tree");
        numNodesAtLevel /= 2;
        currentLevel--;
    }
    if (numNodesAtLevel < 1)
        throw std::invalid_argument("Under-full Huffman code tree");
    if (numNodesAtLevel > 1)
        throw std::invalid_argument("Over-full Huffman code tree");
    return true;
}

map<int, string> genCanonCode(vector<int> codeLength)
{
    // Check basic validity
    if (codeLength.size() < 2)
        throw std::invalid_argument("At least 2 symbols needed");
    if (codeLength.size() > UINT32_MAX)
        throw std::length_error("Too many symbols");

    checkTreeValid(codeLength);
    // sort the codeLength map by value
    vector<pair<int, int>> sortedCodeLength = sort(codeLength);
    map<int, string> canonCode;
    int prevCode = -1;
    int currCode = -1;
    string currCodeStr = "";
    int key = 0;
    int len = 0;
    int prevLen = 0;
    for (auto i = sortedCodeLength.begin(); i != sortedCodeLength.end(); i++)
    {
        key = i->first;
        len = i->second;
        if (len == 0)
        {
            canonCode[key] = "";
            continue;
        }
        if (prevCode == -1)
        {
            currCode = 0;
        }
        else
        {
            currCode = prevCode;
            if (prevLen == len)
            {
                currCode++;
            }
            else
            {
                currCode = 2 * (currCode + 1);
            }
        }
        canonCode[key] = bitset<40>(currCode).to_string().substr(40 - len, len);
        prevCode = currCode;
        prevLen = len;
    }
    return canonCode;
}