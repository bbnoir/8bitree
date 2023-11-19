#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

typedef int8_t int8;

using namespace std;

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

public:
    DataLoader(string filePath); // read data from bin file
    int getNumInts();
    int getNumLines();
    int getIntPerLine();
    int getNumBytes();
    const vector<vector<int8>> &getDataAry();
    vector<int> getFreqMap();
    friend ostream &operator<<(ostream &os, const DataLoader &dl);
};