#include "Encoder.h"
#include "Constants.h"
#include <algorithm>
#include <iostream>
#include <bitset>
#include <fstream>
#include <sstream>

using namespace std;

Encoder::Encoder(DataLoader *dl, TreeArray *tree) : DL(dl), tree(tree) {}

vector<int> Encoder::genCodeLength()
{
    codeLength = vector<int>(SYM_NUM, 0);
    vector<int> freqMap = DL->getFreqMap();
    // sort the freqMap based on the frequency
    vector<pair<int, int8>> freqMapWithIndex;
    // <frequency, number>
    for (int i = 0; i < SYM_NUM; i++)
        freqMapWithIndex.push_back(make_pair(freqMap[i], (int8)i));
    sort(freqMapWithIndex.begin(), freqMapWithIndex.end(), [](pair<int, int8> a, pair<int, int8> b)
         { return a.first > b.first; });
    // build the table
    unordered_map<int8, int> lengthTable;
    vector<short> treeAry = tree->getCodeArray();
    // [nums of 0 bits code, nums of 1 bits code, ...] sum = 16
    int cl = 0;
    // let higher frequency symbol have shorter code length
    for (int j = 0; j < SYM_NUM; j++)
    {
        while (treeAry[cl] == 0)
            cl++;
        lengthTable[freqMapWithIndex[j].second] = cl;
        treeAry[cl]--;
    }
    for (int i = 0; i < SYM_NUM; i++)
        codeLength[i] = lengthTable[i];
    return codeLength;
}

bool cmp(const pair<int, int> &a, const pair<int, int> &b)
{
    if (a.second == b.second)
        return a.first < b.first;
    return a.second < b.second;
}

// Function to sort the map according to value in a (key-value) pairs
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

map<int, string> Encoder::genCanonCode()
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

map<int, string> Encoder::genCanonCode(vector<int> codeLength)
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

void Encoder::encode(string inputFileName, string outputFileName)
{
    ifstream in(inputFileName);
    ofstream out(outputFileName);
    genCodeLength();
    // write the code length
    for (int i = 0; i < SYM_NUM; i++)
        out << codeLength[i] << " ";
    out << endl;
    // write the code
    map<int, string> canonCode = genCanonCode();
    string line;
    while (getline(in, line))
    {
        stringstream ss(line);
        int num;
        while (ss >> num)
            out << canonCode[num];
        out << endl;
    }
    in.close();
    out.close();
}