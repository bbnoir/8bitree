#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

typedef int8_t int8;
typedef pair<int, int> pairInt;

class DataLoader
{
private:
    vector<vector<int8>> dataAry;
    string filePath;
    unsigned int numBytes;
    int numLines;
    int intPerLine; // number of elements per line
    int numInts;    // number of integers occured in the data
    vector<int> freqMap;
    vector<pairInt> occurenceMap;
    vector<pairInt> extremumMap;

public:
    DataLoader(string filePath); // read data from bin file
    int getNumInts();
    int getNumLines();
    int getIntPerLine();
    int getNumBytes();
    const vector<vector<int8>> &getDataAry();
    vector<int> getFreqMap();
    vector<pairInt> getOccurenceMap();
    vector<pairInt> getExtremumMap();
    friend ostream &operator<<(ostream &os, const DataLoader &dl);
};