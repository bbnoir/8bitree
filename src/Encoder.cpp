#include "Encoder.h"
#include "Constants.h"
#include "BitStream.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>
#include <bitset>
#include <fstream>
#include <sstream>

#define THREAD_NUM 8

using namespace std;

Encoder::Encoder(DataLoader *dl, TreeArray *tree) : dl(dl), tree(tree), codeLength(cl_FQ)
{
    bestWidth = INFINITY;
    bestWay = -1;
    cl_FQ = vector<int>(SYM_NUM, 0);
    cl_EX = vector<int>(SYM_NUM, 0);
    cl_OC = vector<int>(SYM_NUM, 0);
}

int Encoder::getMaxWidth(vector<int> lenghtTable)
{
    int numLines = dl->getNumLines();
    int intPerLine = dl->getIntPerLine();
    vector<int> result(numLines, 0);
#pragma omp parallel for num_threads(THREAD_NUM)
    for (int i = 0; i < numLines; i++)
        for (int j = 0; j < intPerLine; j++)
            result[i] += lenghtTable[dl->getDataAry()[i][j] - SYM_MIN];
    return *max_element(result.begin(), result.end());
}

int Encoder::genCodeLength_FQ()
{
    cl_FQ = vector<int>(SYM_NUM, 0);
    vector<freq_t> freqMap = dl->getFreqMap();
    // sort the freqMap based on the frequency
    vector<pair<freq_t, int8>> freqMapWithIndex;
    // <frequency, number>
    for (int i = 0; i < SYM_NUM; i++)
        freqMapWithIndex.push_back(make_pair(freqMap[i], (int8)i));
    sort(freqMapWithIndex.begin(), freqMapWithIndex.end(), [](pair<freq_t, int8> a, pair<freq_t, int8> b)
         { return a.first > b.first; });
    // build the table
    unordered_map<int8, int> lengthTable;
    vector<short> treeAry = tree->getCodeArray();
    // [nums of 0 bits code, nums of 1 bits code, ...] sum = 16
    int cl = 0;
    // let higher frequency symbol have shorter code length
    for (int j = 0; j < dl->getNumInts(); j++)
    {
        while (treeAry[cl] == 0)
            cl++;
        lengthTable[freqMapWithIndex[j].second] = cl;
        treeAry[cl]--;
    }
    for (int i = 0; i < SYM_NUM; i++)
        cl_FQ[i] = lengthTable[i];
    return getMaxWidth(cl_FQ);
}

int Encoder::genCodeLength_EX()
{
    cl_EX = vector<int>(SYM_NUM, 0);
    vector<pairInt> EXmap = dl->getExtremumMap();
    // sort the EXmap based on the max times of occurence
    vector<pair<pairInt, int8>> extendedMapWithIndex;
    // <occurence, number>
    for (int i = 0; i < SYM_NUM; i++)
        extendedMapWithIndex.push_back(make_pair(EXmap[i], (int8)i));
    sort(extendedMapWithIndex.begin(), extendedMapWithIndex.end(), [](pair<pairInt, int8> a, pair<pairInt, int8> b)
         {
        if(a.first.first == b.first.first)
            return a.first.second > b.first.second;
        return a.first.first > b.first.first; });

    // build the table
    unordered_map<int8, int> lengthTable;
    // progressBar(i+1, trees.size());
    vector<short> treeAry = tree->getCodeArray();
    // [nums of 0 bits code, nums of 1 bits code, ...] sum = 16
    int cl = 0;
    // let higher occurence symbol have shorter code length
    for (int j = 0; j < dl->getNumInts(); j++)
    {
        while (!treeAry[cl])
            cl++;
        lengthTable[extendedMapWithIndex[j].second] = cl;
        treeAry[cl]--;
    }
    for (int i = 0; i < SYM_NUM; i++)
        cl_EX[i] = lengthTable[i];
    return getMaxWidth(cl_EX);
}

int Encoder::genCodeLength_OC()
{
    cl_OC = vector<int>(SYM_NUM, 0);
    vector<pairInt> OCmap = dl->getOccurenceMap();
    // sort the OCmap based on the times of occurence
    vector<pair<pairInt, int8>> occurenceMapWithIndex;
    // <occurence, number>
    for (int i = 0; i < SYM_NUM; i++)
        occurenceMapWithIndex.push_back(make_pair(OCmap[i], (int8)i));
    sort(occurenceMapWithIndex.begin(), occurenceMapWithIndex.end(), [](pair<pairInt, int8> a, pair<pairInt, int8> b)
         {
        if(a.first.first == b.first.first)
            return a.first.second < b.first.second;
        return a.first.first > b.first.first; });

    // build the table
    unordered_map<int8, int> lengthTable;
    // progressBar(i+1, trees.size());
    vector<short> treeAry = tree->getCodeArray();
    // [nums of 0 bits code, nums of 1 bits code, ...] sum = 16
    int cl = 0;
    // let higher occurence symbol have shorter code length
    for (int j = 0; j < dl->getNumInts(); j++)
    {
        while (!treeAry[cl])
            cl++;
        lengthTable[occurenceMapWithIndex[j].second] = cl;
        treeAry[cl]--;
    }
    for (int i = 0; i < SYM_NUM; i++)
        cl_OC[i] = lengthTable[i];
    return getMaxWidth(cl_OC);
}

int Encoder::genCodeLength()
{
    bestWidth = INFINITY;
    int width;
    width = genCodeLength_FQ();
    if (width < bestWidth)
    {
        bestWidth = width;
        codeLength = cl_FQ;
        bestWay = FQ;
    }
    width = genCodeLength_EX();
    if (width < bestWidth)
    {
        bestWidth = width;
        codeLength = cl_EX;
        bestWay = EX;
    }
    width = genCodeLength_OC();
    if (width < bestWidth)
    {
        bestWidth = width;
        codeLength = cl_OC;
        bestWay = OC;
    }
    return bestWidth;
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

map<int, pair<int, int>> Encoder::genCanonCode()
{
    // Check basic validity
    if (codeLength.size() < 2)
        throw std::invalid_argument("At least 2 symbols needed");
    if (codeLength.size() > UINT32_MAX)
        throw std::length_error("Too many symbols");
    checkTreeValid(codeLength);
    // sort the codeLength map by value
    vector<pair<int, int>> sortedCodeLength = sort(codeLength);
    map<int, pair<int, int>> canonCode;
    int prevCode = -1;
    int currCode = -1;
    string currCodeStr = "";
    int key = 0;
    int len = 0;
    int prevLen = 0;
    // start generating canonCode
    for (auto i = sortedCodeLength.begin(); i != sortedCodeLength.end(); i++)
    {
        key = i->first;
        len = i->second;
        if (len == 0)
        {
            canonCode[key].first = 0;
            canonCode[key].second = 0;
            continue;
        }
        if (prevCode == -1)
        {
            currCode = 0;
        }
        else
        {
            currCode = (prevCode + 1) << (len - prevLen);
        }
        canonCode[key].first = currCode;
        canonCode[key].second = len;
        prevCode = currCode;
        prevLen = len;
    }
    return canonCode;
}

void Encoder::encode(string outputFileName)
{
    BitOutputStream out(outputFileName);
    int intPerLine = dl->getIntPerLine();
    int numLines = dl->getNumLines();
    out.writeInt(intPerLine);
    out.writeInt(numLines);
    genCodeLength();
    // write the code length
    for (int i = 0; i < SYM_NUM; i++)
        out.writeInt(codeLength[i]);
    // write the code
    map<int, pair<int, int>> canonCode = genCanonCode();
    for (int i = 0; i < numLines; i++)
    {
        for (int j = 0; j < intPerLine; j++)
        {
            out.writeCode(canonCode[dl->getDataAry()[i][j]].first, canonCode[dl->getDataAry()[i][j]].second);
        }
    }
}

int Encoder::getBestWidth()
{
    bestWidth = genCodeLength();
    return bestWidth;
}

int Encoder::getBestWay()
{
    return bestWay;
}