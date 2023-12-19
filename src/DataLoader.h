#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

#define FREQ_MAX UINT32_MAX
typedef int8_t int8;
typedef std::uint32_t freq_t;
typedef pair<int, int> pairInt;

class DataLoader
{
private:
    string filePath;
    vector<vector<int8>> dataAry;
    unsigned int numBytes;
    int numLines;
    int elementPerLine; // number of elements per line
    int numInts;        // number of integers occured in the data
    vector<freq_t> freqMap;
    vector<pairInt> occurenceMap;
    vector<pairInt> extremumMap;

public:
    DataLoader(string filePath); // read data from bin file
    int getNumInts();
    int getNumLines();
    int getElementPerLine();
    int getNumBytes();
    const vector<vector<int8>> &getDataAry();
    vector<freq_t> getFreqMap();
    vector<pairInt> getOccurenceMap();
    vector<pairInt> getExtremumMap();
    friend ostream &operator<<(ostream &os, const DataLoader &dl);
};
