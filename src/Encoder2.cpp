#include "Encoder2.h"
#include "Constants.h"
#include "BitStream.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>
#include <bitset>
#include <fstream>
#include <sstream>

#define THREAD_NUM 12

using namespace std;

Encoder2::Encoder2(DataLoader *dl, vector<int> &codeLength) : dl(dl), codeLength(codeLength)
{
    bestWidth = INT32_MAX;
}

int Encoder2::getMaxWidth()
{
    int numLines = dl->getNumLines();
    int intPerLine = dl->getElementPerLine();
    vector<int> result(numLines, 0);
#pragma omp parallel for num_threads(THREAD_NUM)
    for (int i = 0; i < numLines; i++)
        for (int j = 0; j < intPerLine; j++)
            result[i] += codeLength[dl->getDataAry()[i][j] - SYM_MIN];
    return *max_element(result.begin(), result.end());
}

map<int, string> Encoder2::genCanonCode()
{
    // Check basic validity
    if (codeLength.size() < 2)
        throw std::invalid_argument("At least 2 symbols needed");
    if (codeLength.size() > UINT32_MAX)
        throw std::length_error("Too many symbols");
    checkTreeValid(codeLength);
    // sort the codeLength map by value
    vector<pair<int, int>> sortedCodeLength = sort_cl(codeLength);
    map<int, string> canonCode;
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
            canonCode[key] = "";
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
        canonCode[key] = bitset<ARRAY_SIZE>(currCode).to_string().substr(ARRAY_SIZE - len);
        prevCode = currCode;
        prevLen = len;
    }
    return canonCode;
}

void Encoder2::encode(string outputFileName)
{
    BitOutputStream *out = new BitOutputStream(outputFileName);
    int intPerLine = dl->getElementPerLine();
    int numLines = dl->getNumLines();
    out->writeInt(intPerLine);
    out->writeInt(numLines);
    // write the code length
    for (int i = 0; i < SYM_NUM; i++)
        out->writeInt(codeLength[i]);
    // write the code
    map<int, string> canonCode = genCanonCode();
    for (int i = 0; i < numLines; i++)
    {
        for (int j = 0; j < intPerLine; j++)
        {
            out->writeCode(canonCode[dl->getDataAry()[i][j]]);
        }
    }
    delete out;
}
